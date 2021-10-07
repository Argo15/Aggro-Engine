#include "GBuffer.hpp"
#include "Texture.hpp"
#include "Locks.hpp"
#include "RGBImage.hpp"
#include "CommandTree/GBuffer/InitializeGBuffer.hpp"
#include "CommandTree/GBuffer/Filter/FilterLayer.hpp"
#include "CommandTree/GBuffer/DepthTest/DepthTest.hpp"
#include "CommandTree/GBuffer/DrawElements/DrawElements.hpp"
#include "CommandTree/GBuffer/HasTangents/HasTangents.hpp"
#include "CommandTree/GBuffer/Matrix/MatrixLayer.hpp"
#include "CommandTree/GBuffer/Texture/TextureBinding.hpp"
#include "CommandTree/GBuffer/Material/MaterialLayer.hpp"

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
	m_commands->addLayer(shared_ptr<Layer>(new HasTangents()));
	m_commands->addLayer(shared_ptr<Layer>(new MatrixLayer(m_glslProgram)));
	m_commands->addLayer(shared_ptr<Layer>(new MaterialLayer(m_glslProgram)));
	m_commands->addLayer(shared_ptr<Layer>(new TextureBinding(m_glslProgram, m_syncContext, m_whiteTexture)));
	m_commands->addLayer(shared_ptr<Layer>(new DrawElements(m_glslProgram)));
}

void GBuffer::drawToBuffer(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain)
{
	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);
	m_commands->execute(renderOptions, renderChain);
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