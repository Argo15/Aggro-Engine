#include "ShadowMapBuffer.hpp"
#include "Config.hpp"

ShadowMapBuffer::ShadowMapBuffer(OpenGL43Graphics *graphics, int defaultSize)
	: FrameBufferObject(defaultSize, defaultSize)
{
	const Properties& props = gConfig->getProperties();
	m_size = props.getIntProperty("graphics.shadowRes", defaultSize);
	m_width = m_size;
	m_height = m_size;

	vector<float> configSlices = props.getFloatArrayProperty("graphics.shadowSlices");
	m_slices = new float[5] { 0, configSlices[0], configSlices[1], configSlices[2], 1.0f };

	m_graphics = graphics;

	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	m_glslProgram = graphics->getShaderStore().getShader("Resources/Shaders/v_shadow.glsl", "Resources/Shaders/f_dshadow.glsl");

	glEnable(GL_TEXTURE_2D);

	for (int i = 0; i < 4; i++)
	{
		shared_ptr<Image> fboImage((new Image(m_width, m_height, boost::shared_array<unsigned char>(0)))
			->setImageFormat(ImageFormat::RGBA)
		);
		shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(fboImage));

		glGenFramebuffers(1, &m_shadowBuffer[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer[i]);

		// Generate test texture
		fboImage->setImageType(ImageType::FLOAT_TYPE);
		texOptions->genMipmaps(false)
			->setWrap(Wrap::CLAMP_TO_EDGE)
			->setInternalFormat(InternalFormat::RGBA8);
		m_testTex[i] = graphics->createTexture(texOptions);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_testTex[i]->get(), 0);

		// Generate depth texture
		fboImage->setImageFormat(ImageFormat::DEPTH_COMPONENT);
		texOptions->setInternalFormat(InternalFormat::DEPTH_COMPONENT32)->enableDepthCompare();
		m_shadowTex[i] = graphics->createTexture(texOptions);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowTex[i]->get(), 0);

		// check FbO status
		GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("ShadowMapBuffer init failed, status:%i\n", FBOstatus);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapBuffer::drawToBuffer(RenderOptions renderOptions, std::queue<shared_ptr<RenderData>> &renderQueue)
{
	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	for (int i = 0; i < 4; i++)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_shadowBuffer[i]);
		m_glslProgram->use();
		GLenum mrt[] = { GL_COLOR_ATTACHMENT0_EXT };
		glDrawBuffers(1, mrt);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, getWidth(), getHeight());
		glLineWidth(1);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		int currentLineWidth = 1;

		shared_ptr<DirectLight> light = renderOptions.getDirectLight();
		if (light)
		{
			glm::vec3 lightDir = light->getDirection();
			glm::vec3 lightUp = light->getDirection() == glm::vec3(0, -1.0f, 0) ? glm::vec3(1.0f) : glm::vec3(0, 1.0f, 0);
			shared_ptr<Frustrum> frustrum = renderOptions.getFrustrum();
			float frustrumLength = frustrum->getZFar() - frustrum->getZNear();

			float slicePctStart = m_slices[i];
			float slicePctEnd = m_slices[i + 1];
			float centerPct = (slicePctStart + slicePctEnd) / 2.0f;

			glm::vec3 center = frustrum->getEyePos() + frustrum->getLookDir() * (frustrum->getZNear() + frustrumLength * centerPct);
			glm::mat4 lightViewMat = glm::lookAt(center + lightDir * -250.0f, center, lightUp);

			_getProjectionMat(i, slicePctStart, slicePctEnd, lightViewMat, renderOptions);
		}

		glBindFragDataLocation(m_glslProgram->getHandle(), 0, "testBuffer");
		glBindAttribLocation(m_glslProgram->getHandle(), 0, "v_vertex");
		std::queue<shared_ptr<RenderData>> copyRenderQueue(renderQueue);
		while (!copyRenderQueue.empty() && light)
		{
			shared_ptr<RenderData> renderData = copyRenderQueue.front();
			copyRenderQueue.pop();

			if (renderData->getVertexBufferHandle() && renderData->isDepthTestEnabled() && renderData->isShadowsEnabled())
			{
				glm::mat4 mvpMatrix = m_viewProj[i] * renderData->getModelMatrix();
				m_glslProgram->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvpMatrix), false, 4);

				shared_ptr<VertexBufferHandle> vboHandle = renderData->getVertexBufferHandle();
				glBindBuffer(GL_ARRAY_BUFFER, vboHandle->getVertexHandle());
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle->getIndexHandle());

				glEnableVertexAttribArray(0);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glDrawElements((GLenum)(renderData->getDrawMode()), vboHandle->getSizeOfIndicies(), GL_UNSIGNED_INT, 0);

				glDisableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
		m_glslProgram->disable();
		unbindFrameBuffer();
		glPopAttrib();
	}
}

void ShadowMapBuffer::_getProjectionMat(int slice, float slicePctStart, float slicePctEnd, glm::mat4 &lightViewMat, RenderOptions &renderOptions)
{
	shared_ptr<Frustrum> frustrum = renderOptions.getFrustrum();
	float frustrumLength = frustrum->getZFar() - frustrum->getZNear();

	float nearDistance = frustrum->getZNear() + slicePctStart * frustrumLength;
	float nearHeight = 2 * tan(frustrum->getFov() / 2) * nearDistance;
	float nearWidth = nearHeight * frustrum->getAspectRatio();

	float farDisance = frustrum->getZNear() + slicePctEnd * frustrumLength;
	float farHeight = 2 * tan(frustrum->getFov() / 2) * farDisance;
	float farWidth = farHeight * frustrum->getAspectRatio();

	glm::vec3 nearCenter = frustrum->getEyePos() + frustrum->getLookDir() * nearDistance;
	glm::vec3 farCenter = frustrum->getEyePos() + frustrum->getLookDir() * farDisance;

	glm::vec3 corners[] = {
		nearCenter + (nearHeight / 2) * frustrum->getUpDir() + (nearWidth / 2) * frustrum->getRightDir(),
		nearCenter - (nearHeight / 2) * frustrum->getUpDir() + (nearWidth / 2) * frustrum->getRightDir(),
		nearCenter + (nearHeight / 2) * frustrum->getUpDir() - (nearWidth / 2) * frustrum->getRightDir(),
		nearCenter - (nearHeight / 2) * frustrum->getUpDir() - (nearWidth / 2) * frustrum->getRightDir(),
		farCenter + (farHeight / 2) * frustrum->getUpDir() + (farWidth / 2) * frustrum->getRightDir(),
		farCenter - (farHeight / 2) * frustrum->getUpDir() + (farWidth / 2) * frustrum->getRightDir(),
		farCenter + (farHeight / 2) * frustrum->getUpDir() - (farWidth / 2) * frustrum->getRightDir(),
		farCenter - (farHeight / 2) * frustrum->getUpDir() - (farWidth / 2) * frustrum->getRightDir()
	};

	glm::mat4 vpMat = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f) * lightViewMat;
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 pos = vpMat * glm::vec4(corners[i], 1.0);
		minX = min(minX, pos.x);
		maxX = max(maxX, pos.x);
		minY = min(minY, pos.y);
		maxY = max(maxY, pos.y);
	}

	glm::mat4 lightProjMat = glm::ortho(minX, maxX, minY, maxY, 0.1f, 500.f);
	m_viewProj[slice] = lightProjMat * lightViewMat;
	m_adjViewProj[slice] = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) * m_viewProj[slice];
}

void ShadowMapBuffer::bindShadowTex(int slice)
{
	glBindTexture(GL_TEXTURE_2D, m_shadowTex[slice]->get());
}

shared_ptr<TextureHandle> ShadowMapBuffer::getShadowTex(int slice)
{
	return m_shadowTex[slice];
}

void ShadowMapBuffer::bindTestTex(int slice)
{
	glBindTexture(GL_TEXTURE_2D, m_testTex[slice]->get());
}

shared_ptr<TextureHandle> ShadowMapBuffer::getTestTex(int slice)
{
	return m_testTex[slice];
}

glm::mat4 ShadowMapBuffer::getViewProjection(int slice)
{
	return m_viewProj[slice];
}

float *ShadowMapBuffer::getAdjViewProjectionPointer(int slice)
{
	return glm::value_ptr(m_adjViewProj[slice]);
}

int ShadowMapBuffer::getSize()
{
	return m_size;
}

float ShadowMapBuffer::getSizeF()
{
	return (float)m_size;
}