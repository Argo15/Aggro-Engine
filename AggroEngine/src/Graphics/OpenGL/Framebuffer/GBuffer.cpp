#include "GBuffer.hpp"
#include "DevTexture.hpp"
#include "Locks.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
unique_ptr<DevTexture> devTexture;

GBuffer::GBuffer(OpenGL43Graphics *graphics, int width, int height)
{
	m_width = width;
	m_height = height;
	m_graphics = graphics;

	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	std::string log;
	m_gBufferProg = graphics->getShaderStore().getShader("Resources/Shaders/v_GBuffer.glsl", "Resources/Shaders/f_GBuffer.glsl");


	glEnable(GL_TEXTURE_2D);

	glGenFramebuffers(1, &m_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

	shared_ptr<Image> fboImage(new Image(width, height, ImageFormat::RGBA, InternalFormat::RGBA16, 0, boost::shared_array<unsigned char>(0)));
	fboImage->setImageType(ImageType::FLOAT_TYPE);
	shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(fboImage));
	texOptions->genMipmaps(false)->setWrap(Wrap::CLAMP_TO_EDGE);

	// Generate normal
	m_normalTex = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_normalTex->get(), 0);

	// Generate selection
	m_selectionTex = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_selectionTex->get(), 0);

	// Generate albedo
	fboImage->setInternalFormat(InternalFormat::RGBA8);
	m_albedoTex = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_albedoTex->get(), 0);

	// Generate depth texture
	fboImage->setImageFormat(ImageFormat::DEPTH_COMPONENT)->setInternalFormat(InternalFormat::DEPTH_COMPONENT32);
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

	devTexture = unique_ptr<DevTexture>(new DevTexture(32, 32));
}

void GBuffer::drawToBuffer(RenderOptions renderOptions, std::queue<shared_ptr<RenderData>> &renderQueue)
{
	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);
	m_gBufferProg->use();

	bind();
	GLenum mrt[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glDrawBuffers(3, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, getWidth(), getHeight());

	glBindFragDataLocation(m_gBufferProg->getHandle(), 0, "normalBuffer");
	glBindFragDataLocation(m_gBufferProg->getHandle(), 1, "albedoBuffer");
	glBindFragDataLocation(m_gBufferProg->getHandle(), 2, "selectionBuffer");
	glBindAttribLocation(m_gBufferProg->getHandle(), 0, "v_a_vertex");
	glBindAttribLocation(m_gBufferProg->getHandle(), 1, "v_b_texcoord");
	glBindAttribLocation(m_gBufferProg->getHandle(), 2, "v_c_normal");

	while (!renderQueue.empty())
	{
		shared_ptr<RenderData> renderData = renderQueue.front();
		renderQueue.pop();

		if (renderData->getVertexBufferHandle() != nullptr)
		{
			glm::mat4 mvpMatrix = renderOptions.getProjectionMatrix()*renderOptions.getViewMatrix()*renderData->getModelMatrix();
			m_gBufferProg->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvpMatrix), false, 4);
			glm::mat3 normalMatrix = glm::mat3(renderData->getModelMatrix());
			m_gBufferProg->sendUniform("normalMatrix", glm::value_ptr(normalMatrix), false, 3);

			glActiveTexture(GL_TEXTURE0);
			if (renderData->getTextureHandle().get() != nullptr)
			{
				glBindTexture(GL_TEXTURE_2D, renderData->getTextureHandle()->get());
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, devTexture->getHandle()->get());
			}
			m_gBufferProg->sendUniform("tex", 0);
			unsigned int id = renderData->getId();
			float r = (id % 255) / 255.0f;
			float g = ((id / 255) % 255) / 255.0f;
			float b = ((id / 65025) % 255) / 255.0f;
			m_gBufferProg->sendUniform("objId", r, g, b);

			shared_ptr<VertexBufferHandle> vboHandle = renderData->getVertexBufferHandle();
			glBindBuffer(GL_ARRAY_BUFFER, vboHandle->getVertexHandle());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle->getIndexHandle());

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies()));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies() * 5 / 3));
			glDrawElements((GLenum)(renderData->getDrawMode()), vboHandle->getSizeOfIndicies(), GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			Texture::unbind();
		}
	}
	unbind();
	glPopAttrib();
}

void GBuffer::bind() 
{ 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_buffer); m_gBufferProg->use(); 
}

void GBuffer::unbind() 
{ 
	m_gBufferProg->disable(); 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
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

shared_ptr<GLSLProgram> GBuffer::getProgram()
{ 
	return m_gBufferProg; 
}

int GBuffer::getWidth() 
{ 
	return m_width; 
}

int GBuffer::getHeight() 
{ 
	return m_height; 
}