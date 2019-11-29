#include "OpenGL43Graphics.hpp"
#include "DefaultTextureHandle.hpp"
#include "DefaultVertexBufferHandle.hpp"
#include "Grid.hpp"
#include "Config.hpp"
#include "PerfStats.hpp"
#include "Locks.hpp"
#include "RGBImage.hpp"
#include "Screen.hpp"
#include "GLDebug/DebugCallback.hpp"
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

OpenGL43Graphics::OpenGL43Graphics()
{
}

OpenGL43Graphics::~OpenGL43Graphics()
{
}

void OpenGL43Graphics::init(shared_ptr<GraphicsInitOptions> options)
{
	this->lock();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)_debugCallback, 0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_syncContext = shared_ptr<BufferSyncContext>(new BufferSyncContext());
	this->unlock();
	m_shadowBuffer = shared_ptr<ShadowMapBuffer>(new ShadowMapBuffer(this, options->getShadowSize()));
	m_gBuffer = shared_ptr<GBuffer>(new GBuffer(this, options->getBufferWidth(), options->getBufferHeight()));
	m_lightBuffer = shared_ptr<LightBuffer>(new LightBuffer(this, options->getBufferWidth(), options->getBufferHeight()));
	m_shadedBuffer = shared_ptr<ShadedBuffer>(new ShadedBuffer(this, options->getBufferWidth(), options->getBufferHeight()));
	m_viewport = shared_ptr<Viewport>(new Viewport());
	m_screenVBO = createVertexBuffer(shared_ptr<Mesh>(new Screen(-1, 0, 0, 1, 1)));
	m_screenProgram = getShaderStore().getShader("Resources/Shaders/v_screen.glsl", "Resources/Shaders/f_screen.glsl");
	m_pixelBuffers = shared_ptr<PixelBufferCache>(new PixelBufferCache(options));
}


shared_ptr<VertexBufferHandle> OpenGL43Graphics::createVertexBuffer(shared_ptr<Mesh> mesh)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	GLuint nVertexHandle;
	glGenBuffers(1, &nVertexHandle);
	glBindBuffer(GL_ARRAY_BUFFER, nVertexHandle);
	// size = 14 for 3 vert, 2 tex, 3 norm, 3 tan, 3 bitan
	int bufferSize = mesh->hasTangents() ? mesh->getSizeOfVerticies() * 14 / 3 : mesh->getSizeOfVerticies() * 8 / 3;
	glBufferData(GL_ARRAY_BUFFER,mesh->getSizeOfVerticies()*14/3,NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,mesh->getSizeOfVerticies(),mesh->getVerticies().get());
	glBufferSubData(GL_ARRAY_BUFFER,mesh->getSizeOfVerticies(),mesh->getSizeOfVerticies()*2/3,mesh->getTexCoords().get());
	glBufferSubData(GL_ARRAY_BUFFER,mesh->getSizeOfVerticies()*5/3,mesh->getSizeOfVerticies(),mesh->getNormals().get());
	if (mesh->hasTangents())
	{
		glBufferSubData(GL_ARRAY_BUFFER, mesh->getSizeOfVerticies() * 8 / 3, mesh->getSizeOfVerticies(), mesh->getTangents().get());
		glBufferSubData(GL_ARRAY_BUFFER, mesh->getSizeOfVerticies() * 11 / 3, mesh->getSizeOfVerticies(), mesh->getBitangents().get());
	}

	GLuint nIndexHandle;
	glGenBuffers(1, &nIndexHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nIndexHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getSizeOfIndicies(), mesh->getIndicies().get(), GL_STATIC_DRAW);
	m_syncContext->addSync(nVertexHandle, glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0));
	return shared_ptr<VertexBufferHandle>(new DefaultVertexBufferHandle(nVertexHandle, mesh->getSizeOfVerticies(), nIndexHandle, mesh->getSizeOfIndicies(), mesh->hasTangents()));
}

void OpenGL43Graphics::deleteVertexBuffer(shared_ptr<VertexBufferHandle> nVertexBufferHandle)
{
	GLuint nVBOHandle = nVertexBufferHandle->getVertexHandle();
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glDeleteBuffers(1, &nVBOHandle);
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture()
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	return createTexture(shared_ptr<ImageUC>(new RGBImage(1, 1, glm::vec3(1.f, 1.f, 1.f))));

}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture(shared_ptr<ImageUC> image)
{
	return createTexture(shared_ptr<TextureBuildOptions>(new TextureBuildOptions(image)));
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture(shared_ptr<TextureBuildOptions> pTexOptions)
{
	// TODO this method is slow, better to 
	// 1. glMapBuffer a PBO, 
	// 2. write data to memory map in a second thread
	// 3. flush PBO (through glUnmapBuffer)
	// 4. create texture from PBO
	// 5. wait for sync
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	shared_ptr<ImageUC> pImage = pTexOptions->getImage();
	GLuint m_nHandle;
	glGenTextures(1, &m_nHandle);
	GLenum target = pTexOptions->getTarget();
	glBindTexture(target, m_nHandle);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, pTexOptions->getMagFilter());
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, pTexOptions->getMinFilter());
	glTexParameteri(target, GL_TEXTURE_WRAP_S, pTexOptions->getWrapS());
	glTexParameteri(target, GL_TEXTURE_WRAP_T, pTexOptions->getWrapT());
	if (pTexOptions->isGenMipmaps())
	{
		gluBuild2DMipmaps(target, pTexOptions->getComponents(), pImage->getWidth(), pImage->getHeight(), pImage->getFormat(), pImage->getImageType(), pImage->getData().get());
	}
	else
	{
		glTexImage2D(target, 0, pTexOptions->getInternalFormat(), pImage->getWidth(), pImage->getHeight(), 0, pImage->getFormat(), pImage->getImageType(), 0);
	}
	if (pTexOptions->isDepthCompareEnabled())
	{
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(target, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	}
 	return shared_ptr<TextureHandle>(new DefaultTextureHandle(m_nHandle));
}
	
void OpenGL43Graphics::deleteTexture(shared_ptr<TextureHandle> textureHandle)
{
	GLuint nHandle = textureHandle->get();
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glDeleteTextures(1,&nHandle);
}

void OpenGL43Graphics::stageRender(shared_ptr<RenderData> pRenderData)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	renderQueue.push(pRenderData);
}

void OpenGL43Graphics::executeRender(RenderOptions &renderOptions)
{
	auto tracker = PerfStats::instance().trackTime("executeRender");
	m_shadowBuffer->drawToBuffer(renderOptions, renderQueue, m_syncContext);
	m_gBuffer->drawToBuffer(renderOptions, renderQueue, m_syncContext);
	m_pixelBuffers->writeSelectionBuffer(m_gBuffer);
	m_lightBuffer->drawToBuffer(renderOptions, m_gBuffer->getNormalTex(), m_gBuffer->getDepthTex(), m_gBuffer->getGlowTex(), m_shadowBuffer);
	m_shadedBuffer->drawToBuffer(renderOptions, m_gBuffer->getAlbedoTex(), m_lightBuffer->getTexture(), m_lightBuffer->getGlowTex());
}

void OpenGL43Graphics::drawScreen(RenderOptions &renderOptions)
{
	return _drawScreen(renderOptions, 0.0, 0.0, 1.0, 1.0);
}

void OpenGL43Graphics::setViewport(int nX, int nY, int nWidth, int nHeight)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	m_viewport->setDimensions(nX, nY, nWidth, nHeight);
	glViewport(nX, nY, nWidth, nHeight);
}

shared_ptr<Viewport> OpenGL43Graphics::getViewport()
{
	return m_viewport;
}

void OpenGL43Graphics::clearColor(int frameBufferId)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glClearColor(0.3f, 0.3f, 0.3f, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepth(int frameBufferId)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepthAndColor(int frameBufferId)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glClearColor(0.3f, 0.3f, 0.3f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

ShaderStore OpenGL43Graphics::getShaderStore()
{
	return m_shaderStore;
}

void OpenGL43Graphics::_drawScreen(RenderOptions &renderOptions, float nX1, float nY1, float nX2, float nY2)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderOptions.getDefaultFrameBufferId());
	clearDepth(renderOptions.getDefaultFrameBufferId());
	glClearColor(1.0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	for (int i = 31; i >= 0; i--)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glDisable(GL_TEXTURE_2D);
	}
	glEnable(GL_TEXTURE_2D);

	m_screenProgram->use();
	glPushAttrib(GL_VIEWPORT_BIT);
	glm::vec4 viewport = renderOptions.getViewport();
	setViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glEnable(GL_DEPTH_TEST);

	glBindAttribLocation(m_screenProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(m_screenProgram->getHandle(), 1, "v_texcoord");
	m_screenProgram->sendUniform("texture", _getRenderTargetTexture(renderOptions.getRenderTarget()), 0);

	glm::mat4 mvpMatrix = glm::ortho(0, 1, 0, 1);
	m_screenProgram->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvpMatrix), false, 4);

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
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Texture::unbind();
	m_screenProgram->disable();
	glPopAttrib();
}

shared_ptr<TextureHandle> OpenGL43Graphics::_getRenderTargetTexture(RenderOptions::RenderTarget target)
{
	switch (target)
	{
		case RenderOptions::SHADED:
			return m_shadedBuffer->getTexture();
		case RenderOptions::ALBEDO:
			return m_gBuffer->getAlbedoTex();
		case RenderOptions::NORMAL:
			return m_gBuffer->getNormalTex();
		case RenderOptions::LIGHTING:
			return m_lightBuffer->getTexture();
		case RenderOptions::GLOW:
			return m_lightBuffer->getGlowTex();
		case RenderOptions::SELECTION:
			return m_gBuffer->getSelectionTex();
		case RenderOptions::DIRECT_LIGHT:
			return m_shadowBuffer->getTestTex(0);
		case RenderOptions::DEPTH:
			return m_gBuffer->getDepthTex();
		default:
			break;
	}
	return m_gBuffer->getAlbedoTex();
}

shared_ptr<ImageUC> OpenGL43Graphics::getSelectionImage(int x, int y, int width, int height)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	return m_pixelBuffers->getSelectionImage(x, y, width, height);
}

shared_ptr<ImageF> OpenGL43Graphics::getDepthImage(int x, int y, int width, int height)
{
	// This will flush the command pipeline, and PBOs can't fix it, use only when necessary
	// TODO: do a raycast to get depth
	boost::lock_guard<OpenGL43Graphics> guard(*this);

	m_gBuffer->bindFrameBufferReadOnly();
	int size = width * height;
	float *pixels = new float[size];

	glReadBuffer(GL_NONE);
	glReadPixels(x, y, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
	m_gBuffer->unbindFrameBufferReadOnly();

	return shared_ptr<ImageF>((new ImageF(width, height, mem::shared_array<float>(pixels, size, "GL_Graphics")))
		->setImageFormat(ImageFormat::DEPTH_COMPONENT)
		->setImageType(ImageType::FLOAT_TYPE)
		);
}

shared_ptr<unsigned char> OpenGL43Graphics::getSelectionImagePixel(int x, int y)
{
	shared_ptr<ImageUC> image = getSelectionImage(x, y, 1, 1);
	if (image)
	{
		return image->getPixel(0, 0);
	}
	return shared_ptr<unsigned char>();
}

shared_ptr<float> OpenGL43Graphics::getDepthImagePixel(int x, int y)
{
	shared_ptr<ImageF> image = getDepthImage(x, y, 1, 1);
	if (image)
	{
		return image->getPixel(0, 0);
	}
	return shared_ptr<float>();
}

int OpenGL43Graphics::getFrameBufferWidth()
{
	return m_gBuffer->getWidth();
}

int OpenGL43Graphics::getFrameBufferHeight()
{
	return m_gBuffer->getHeight();
}