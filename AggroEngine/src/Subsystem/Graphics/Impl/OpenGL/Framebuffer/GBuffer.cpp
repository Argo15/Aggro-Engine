#include "GBuffer.hpp"
#include "Texture.hpp"
#include "Locks.hpp"
#include "RGBImage.hpp"
#include "InitializeGBuffer.hpp"
#include "FilterLayer.hpp"
#include "DepthTest.hpp"
#include "DrawElements.hpp"

static glm::vec3 defaultColor(1.0);
static glm::vec3 defaultEmission(0);

GBuffer::GBuffer(OpenGL43Graphics *graphics, int width, int height, shared_ptr<BufferSyncContext> syncContext)
	: FrameBufferObject(width, height)
	, m_syncContext(syncContext)
{
	m_graphics = graphics;

	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	m_glslProgram = graphics->getShaderStore().getShader("Resources/Shaders/v_GBuffer.glsl", "Resources/Shaders/f_GBuffer.glsl");

	glEnable(GL_TEXTURE_2D);
	
	glGenFramebuffers(1, &m_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

	shared_ptr<ImageUC> fboImage((new ImageUC(width, height, mem::shared_array<unsigned char>(0, 0, "GL_Graphics")))
		->setImageFormat(ImageFormat::RGBA)
	);
	fboImage->setImageType(ImageType::FLOAT_TYPE);
	shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(fboImage));

	// Generate 16 bit-per-component here
	texOptions->genMipmaps(false)->setWrap(Wrap::CLAMP_TO_EDGE)
		->setInternalFormat(InternalFormat::RGBA16);

	// Generate normal
	m_normalTex = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_normalTex->get(), 0);

	// Generate 8 bit-per-component here
	texOptions->setInternalFormat(InternalFormat::RGBA8);

	// Generate albedo
	m_albedoTex = graphics->createTexture(texOptions);
	m_texture = m_albedoTex;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_albedoTex->get(), 0);

	// Generate glow texture
	m_glowTex = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_glowTex->get(), 0);

	// Generate selection
	fboImage->setImageType(ImageType::UNSIGNED_BYTE);
	m_selectionTex = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, getSelectionColorAttachment(), GL_TEXTURE_2D, m_selectionTex->get(), 0);

	// Generate depth texture
	fboImage->setImageFormat(ImageFormat::DEPTH_COMPONENT);
	texOptions->setInternalFormat(InternalFormat::DEPTH_COMPONENT24);
	m_depthTex = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTex->get(), 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// check FbO status
	GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("GBuffer init failed, status:%i\n", FBOstatus);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_whiteTexture = graphics->createTexture(shared_ptr<ImageUC>(new RGBImage(1, 1, glm::vec3(1.f, 1.f, 1.f))));

	m_commands = shared_ptr<CommandTree>(new CommandTree());
	m_commands->addLayer(shared_ptr<Layer>(new InitializeGBuffer(this)));
	m_commands->addLayer(shared_ptr<Layer>(new FilterLayer(m_syncContext)));
	m_commands->addLayer(shared_ptr<Layer>(new DepthTest()));
	m_commands->addLayer(shared_ptr<Layer>(new DrawElements(m_syncContext, m_glslProgram, m_whiteTexture)));
}

void GBuffer::drawToBuffer(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain)
{
	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);
	m_commands->execute(renderOptions, renderChain);
}

void GBuffer::drawToBufferOld(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain)
{
	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	bindFrameBufferWriteOnly();
	m_glslProgram->use();
	GLenum mrt[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
	glDrawBuffers(4, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, getWidth(), getHeight());
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	int currentLineWidth = 1;
	bool isDepthDisabled = false;
	shared_ptr<RenderData> disabledDepthObject; // needs to be rendered at the end
	const shared_ptr<Frustrum> frustrum = renderOptions.getFrustrum();
	glm::mat4 viewProj = renderOptions.getProjectionMatrix() * renderOptions.getViewMatrix();
	std::deque<shared_ptr<RenderData>> disabledDepthQueue = std::deque<shared_ptr<RenderData>>();
	shared_ptr<RenderNode> chainNode = renderChain->getFirst();

	glBindFragDataLocation(m_glslProgram->getHandle(), 0, "normalBuffer");
	glBindFragDataLocation(m_glslProgram->getHandle(), 1, "albedoBuffer");
	glBindFragDataLocation(m_glslProgram->getHandle(), 2, "selectionBuffer");
	glBindFragDataLocation(m_glslProgram->getHandle(), 3, "glowBuffer");
	glBindAttribLocation(m_glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(m_glslProgram->getHandle(), 1, "v_texcoord");
	glBindAttribLocation(m_glslProgram->getHandle(), 2, "v_normal");
	glBindAttribLocation(m_glslProgram->getHandle(), 3, "v_tangent");
	glBindAttribLocation(m_glslProgram->getHandle(), 4, "v_bitangent");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	while (chainNode || !disabledDepthQueue.empty())
	{
		shared_ptr<RenderData> renderData;
		if (chainNode) {
			renderData = chainNode->getRenderData();
			chainNode = chainNode->next();
		} else {
			renderData = disabledDepthQueue.front();
			disabledDepthQueue.pop_front();
		}

		shared_ptr<VertexBufferHandle> vboHandle = renderData->getVertexBufferHandle();
		if (vboHandle && !m_syncContext->checkAndClearSync(vboHandle->getVertexHandle()))
		{
			continue;
		}


		if (renderData->isCullingEnabled() && renderData->getOcclusionPoints())
		{
			FrustrumCulling culling = frustrum->getCulling(
				renderData->getOcclusionPoints(),
				renderData->getOcclusionSize(),
				renderData->getModelMatrix());
			if (culling == OUTSIDE)
			{
				continue;
			}
		}

		if (renderData == disabledDepthObject)
		{
			glDisable(GL_DEPTH_TEST);
			isDepthDisabled = true;
		}
		else if (!isDepthDisabled && !renderData->isDepthTestEnabled())
		{
			disabledDepthQueue.push_back(renderData);
			if (!disabledDepthObject)
			{
				disabledDepthObject = renderData;
			}
			continue;
		}

		if (vboHandle)
		{
			glm::mat4 mvpMatrix = viewProj * renderData->getModelMatrix();
			m_glslProgram->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvpMatrix), false, 4);
			glm::mat3 normalMatrix = glm::mat3(renderData->getModelMatrix());
			m_glslProgram->sendUniform("normalMatrix", glm::value_ptr(normalMatrix), false, 3);

			m_glslProgram->sendUniform("lightingEnabled", renderData->isLightingEnabled());

			glm::mat4 textureMatrix = glm::mat4(1.0);
			shared_ptr<Material> material = renderData->getMaterial();
			int texId = 0;
			if (material && m_syncContext->checkAndClearSync(material->getTextureOpt().get_value_or(m_whiteTexture)->get()))
			{
				m_glslProgram->sendUniform("material.color", material->getColor());
				m_glslProgram->sendUniform("material.tex", material->getTextureOpt().get_value_or(m_whiteTexture), texId++);
				m_glslProgram->sendUniform("material.alpha", material->getAlphaOpt().get_value_or(m_whiteTexture), texId++);
				m_glslProgram->sendUniform("material.specIntensity", material->getSpecIntensity());
				m_glslProgram->sendUniform("material.shininess", (float)material->getShininess());
				m_glslProgram->sendUniform("material.specMap", material->getSpecularOpt().get_value_or(m_whiteTexture), texId++);
				m_glslProgram->sendUniform("material.emission", material->getEmission());
				m_glslProgram->sendUniform("material.emissionMap", material->getEmissionMapOpt().get_value_or(m_whiteTexture), texId++);
				boost::optional<shared_ptr<TextureHandle>> normalMap = material->getNormalMapOpt();
				if (normalMap)
				{
					m_glslProgram->sendUniform("material.hasNormals", true);
					m_glslProgram->sendUniform("material.normalMap", normalMap.get(), texId++);
					glm::mat3 texRotateMatrix = material->getTexRotateMatrix();
					texRotateMatrix[0] = glm::vec3(texRotateMatrix[0][0], -texRotateMatrix[0][1], 0);
					texRotateMatrix[1] = glm::vec3(-texRotateMatrix[1][0], texRotateMatrix[1][1], 0);
					texRotateMatrix[2] = glm::vec3(0, 0, 1.0);
					m_glslProgram->sendUniform("texRotateMatrix", glm::value_ptr(texRotateMatrix), false, 3);
				}
				else
				{
					m_glslProgram->sendUniform("material.hasNormals", false);
				}

				textureMatrix = material->getTextureMatrix();
			}
			else
			{
				m_glslProgram->sendUniform("material.color", defaultColor);
				m_glslProgram->sendUniform("material.tex", m_whiteTexture, texId++);
				m_glslProgram->sendUniform("material.alpha", m_whiteTexture, texId++);
				m_glslProgram->sendUniform("material.specIntensity", 0.f);
				m_glslProgram->sendUniform("material.shininess", 0.f);
				m_glslProgram->sendUniform("material.specMap", m_whiteTexture, texId++);
				m_glslProgram->sendUniform("material.emission", defaultEmission);
				m_glslProgram->sendUniform("material.emissionMap", m_whiteTexture, texId++);
				m_glslProgram->sendUniform("material.hasNormals", false);
			}
			m_glslProgram->sendUniform("textureMatrix", glm::value_ptr(textureMatrix), false, 4);


			unsigned int id = renderData->getId();
			float r = (id % 255) / 255.0f;
			float g = ((id / 255) % 255) / 255.0f;
			float b = ((id / 65025) % 255) / 255.0f;
			m_glslProgram->sendUniform("objId", r, g, b);

			if (currentLineWidth != renderData->getLineWidth())
			{
				currentLineWidth = renderData->getLineWidth();
				glLineWidth(currentLineWidth);
			}

			m_glslProgram->sendUniform("hasTangents", vboHandle->hasTangents());

			glBindBuffer(GL_ARRAY_BUFFER, vboHandle->getVertexHandle());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle->getIndexHandle());

			if (vboHandle->hasTangents())
			{
				glEnableVertexAttribArray(3);
				glEnableVertexAttribArray(4);
			}

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies()));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies() * 5 / 3));
			if (vboHandle->hasTangents())
			{
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies() * 8 / 3));
				glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies() * 11 / 3));
			}
			glDrawElements((GLenum)(renderData->getDrawMode()), vboHandle->getSizeOfIndicies(), GL_UNSIGNED_INT, 0);

			if (vboHandle->hasTangents())
			{
				glDisableVertexAttribArray(3);
				glDisableVertexAttribArray(4);
			}

			Texture::unbind();
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	m_glslProgram->disable();
	glPopAttrib();
	unbindFrameBufferWriteOnly();
}

void GBuffer::bindDepthTex() 
{ 
	glBindTexture(GL_TEXTURE_2D, m_depthTex->get());
}

void GBuffer::bindNormalTex() 
{ 
	glBindTexture(GL_TEXTURE_2D, m_normalTex->get());
}

void GBuffer::bindAlbedoTex()
{ 
	glBindTexture(GL_TEXTURE_2D, m_albedoTex->get());
}

void GBuffer::bindSelectionTex()
{
	glBindTexture(GL_TEXTURE_2D, m_selectionTex->get());
}

shared_ptr<TextureHandle> GBuffer::getDepthTex()
{
	return m_depthTex;
}

shared_ptr<TextureHandle> GBuffer::getNormalTex()
{
	return m_normalTex;
}

shared_ptr<TextureHandle> GBuffer::getAlbedoTex()
{
	return m_albedoTex;
}

shared_ptr<TextureHandle> GBuffer::getSelectionTex()
{
	return m_selectionTex;
}

shared_ptr<TextureHandle> GBuffer::getGlowTex()
{
	return m_glowTex;
}

GLenum GBuffer::getSelectionColorAttachment()
{
	return GL_COLOR_ATTACHMENT2;
}