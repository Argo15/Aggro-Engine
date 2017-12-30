#include "ShadedBuffer.hpp"
#include "Texture.hpp"
#include "RGBImage.hpp"
#include "Screen.hpp"

ShadedBuffer::ShadedBuffer(OpenGL43Graphics *graphics, int width, int height)
	: FrameBufferObject(width, height)
{
	m_graphics = graphics;

	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	std::string log;
	m_glslProgram = graphics->getShaderStore().getShader("Resources/Shaders/v_final.glsl", "Resources/Shaders/f_final.glsl");

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffers(1, &m_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

	shared_ptr<Image> fboImage((new Image(width, height, boost::shared_array<unsigned char>(0)))
		->setImageFormat(ImageFormat::RGBA)
	);
	fboImage->setImageType(ImageType::FLOAT_TYPE);
	shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(fboImage));
	texOptions->genMipmaps(false)->setWrap(Wrap::CLAMP_TO_EDGE)->setInternalFormat(InternalFormat::RGBA16);

	// Generate light texture
	m_texture = graphics->createTexture(texOptions);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->get(), 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// check FbO status
	GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("LightBuffer init failed, status:%i\n", FBOstatus);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_screenVBO = graphics->createVertexBuffer(shared_ptr<Mesh>(new Screen(0, 0, 1, 1)));
}

void ShadedBuffer::drawToBuffer(RenderOptions &renderOptions, shared_ptr<TextureHandle> albedoTex, shared_ptr<TextureHandle> lightingTex, shared_ptr<TextureHandle> glowTex)
{
	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	bindFrameBuffer();
	m_glslProgram->use();
	GLenum mrt[] = { GL_COLOR_ATTACHMENT0_EXT };
	glDrawBuffers(1, mrt);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, m_width, m_height);
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);

	glBindFragDataLocation(m_glslProgram->getHandle(), 0, "finalBuffer");
	glBindAttribLocation(m_glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(m_glslProgram->getHandle(), 1, "v_texcoord");

	m_glslProgram->sendUniform("albedoTex", albedoTex, 0);
	m_glslProgram->sendUniform("lightingTex", lightingTex, 1);
	m_glslProgram->sendUniform("glowTex", glowTex, 2);

	glm::mat4 mvpMatrix = glm::ortho(0, 1, 0, 1);
	m_glslProgram->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvpMatrix), false, 4);

	glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO->getVertexHandle());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_screenVBO->getIndexHandle());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_screenVBO->getSizeOfVerticies()));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_screenVBO->getSizeOfVerticies() * 5 / 3));
	glDrawElements(GL_TRIANGLES, m_screenVBO->getSizeOfIndicies(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Texture::unbind();

	m_glslProgram->disable();
	unbindFrameBuffer();
	glPopAttrib();
}