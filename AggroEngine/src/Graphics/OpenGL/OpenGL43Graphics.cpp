#include "OpenGL43Graphics.hpp"
#include "DevTexture.hpp"
#include "Grid.hpp"
#include "Config.hpp"
#include "Locks.hpp"
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

OpenGL43Graphics::OpenGL43Graphics()
{
}

OpenGL43Graphics::~OpenGL43Graphics()
{
}

void OpenGL43Graphics::init()
{
	gLocks->graphics.lock();
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gLocks->graphics.unlock();
	const Properties& props = gConfig->getProperties();
	vector<int> nDimensions = props.getIntArrayProperty("graphics.resolution");
	m_gBuffer = shared_ptr<GBuffer>(new GBuffer(this, nDimensions[0], nDimensions[1]));
	m_pboCache = shared_ptr<PixelBufferCache>(new PixelBufferCache());
	m_viewport = shared_ptr<Viewport>(new Viewport());
}

shared_ptr<VertexBufferHandle> OpenGL43Graphics::createVertexBuffer(shared_ptr<Mesh> mesh)
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	GLuint nVertexHandle;
	glGenBuffers(1, &nVertexHandle);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, nVertexHandle);
	glBufferData(GL_ARRAY_BUFFER,mesh->getSizeOfVerticies()*8/3, NULL,GL_STATIC_DRAW); // 8/3 for 3 vert, 2 tex, 3 norm
	glBufferSubData(GL_ARRAY_BUFFER,0,mesh->getSizeOfVerticies(),mesh->getVerticies().get());
	glBufferSubData(GL_ARRAY_BUFFER,mesh->getSizeOfVerticies(),mesh->getSizeOfVerticies()*2/3,mesh->getTexCoords().get());
	glBufferSubData(GL_ARRAY_BUFFER,mesh->getSizeOfVerticies()*5/3,mesh->getSizeOfVerticies(),mesh->getNormals().get());
	
	GLuint nIndexHandle;
	glGenBuffers(1, &nIndexHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nIndexHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getSizeOfIndicies(), mesh->getIndicies().get(), GL_STATIC_DRAW);
	return shared_ptr<VertexBufferHandle>(new VertexBufferHandle(nVertexHandle, mesh->getSizeOfVerticies(), nIndexHandle, mesh->getSizeOfIndicies()));
}

void OpenGL43Graphics::deleteVertexBuffer(shared_ptr<VertexBufferHandle> nVertexBufferHandle)
{
	GLuint nVBOHandle = nVertexBufferHandle->getVertexHandle();
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	glDeleteBuffers(1, &nVBOHandle);
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture()
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	DevTexture texture(16,16);
	return texture.getHandle();
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture(shared_ptr<TextureBuildOptions> pTexOptions)
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	shared_ptr<Image> pImage = pTexOptions->getImage();
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
		gluBuild2DMipmaps(target, pImage->getComponents(), pImage->getWidth(), pImage->getHeight(), pImage->getFormat(), pImage->getImageType(), pImage->getData().get());
	}
	else
	{
		glTexImage2D(target, 0, pImage->getInternalFormat(), pImage->getWidth(), pImage->getHeight(), 0, pImage->getFormat(), pImage->getImageType(), 0);
	}
 	return shared_ptr<TextureHandle>(new TextureHandle(m_nHandle));
}
	
void OpenGL43Graphics::deleteTexture(shared_ptr<TextureHandle> textureHandle)
{
	GLuint nHandle = textureHandle->get();
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	glDeleteTextures(1,&nHandle);
}

void OpenGL43Graphics::stageTriangleRender(shared_ptr<RenderData> pRenderData)
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	renderQueue.push(pRenderData);
}

void OpenGL43Graphics::executeRender(RenderOptions &renderOptions)
{
	m_gBuffer->drawToBuffer(renderOptions, renderQueue);
	_drawScreen(renderOptions, 0.0, 0.0, 1.0, 1.0);
}

void OpenGL43Graphics::setViewport(int nX, int nY, int nWidth, int nHeight)
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	m_viewport->setDimensions(nX, nY, nWidth, nHeight);
	glViewport(nX, nY, nWidth, nHeight);
}

shared_ptr<Viewport> OpenGL43Graphics::getViewport()
{
	return m_viewport;
}

void OpenGL43Graphics::clearColor()
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepth()
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepthAndColor()
{
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

ShaderStore OpenGL43Graphics::getShaderStore()
{
	return m_shaderStore;
}

void OpenGL43Graphics::_drawScreen(RenderOptions &renderOptions, float nX1, float nY1, float nX2, float nY2)
{
	clearDepth();
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	glDisable(GL_LIGHTING);
	for (int i = 31; i >= 0; i--)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glDisable(GL_TEXTURE_2D);
	}
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1.0, 0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, _getRenderTargetTexture(renderOptions.getRenderTarget())->get());
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);		 glVertex2f(nX1, nY1);
		glTexCoord2f(1.0f, 0);	 glVertex2f(nX2, nY1);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(nX2, nY2);
		glTexCoord2f(0, 1.0f);	 glVertex2f(nX1, nY2);
	glEnd();
}

shared_ptr<TextureHandle> OpenGL43Graphics::_getRenderTargetTexture(RenderOptions::RenderTarget target)
{
	switch (target)
	{
		case RenderOptions::SHADED:
			return m_gBuffer->getAlbedoTex();
		case RenderOptions::ALBEDO:
			return m_gBuffer->getAlbedoTex();
		case RenderOptions::NORMAL:
			return m_gBuffer->getNormalTex();
		case RenderOptions::SELECTION:
			return m_gBuffer->getSelectionTex();
		default:
			break;
	}
	return m_gBuffer->getAlbedoTex();
}

shared_ptr<Image> OpenGL43Graphics::getRenderImage(RenderOptions::RenderTarget target)
{
	int width = m_gBuffer->getWidth();
	int height = m_gBuffer->getHeight();
	return getRenderImage(0, 0, width, height, target);
}

shared_ptr<Image> OpenGL43Graphics::getRenderImage(int x, int y, int width, int height, RenderOptions::RenderTarget target)
{
	int pixelSize = sizeof(unsigned short) * 4;
	int size = width * height * pixelSize;
	unsigned char *pixels = new unsigned char[size];

	int idxStart = (y * m_gBuffer->getWidth() * pixelSize) + (x * pixelSize);
	boost::lock_guard<boost::mutex> guard(gLocks->graphics);
	GLuint pbo = m_pboCache->getPixelBuffer(m_gBuffer->getWidth() * m_gBuffer->getHeight() * pixelSize, "RT");

	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
	glBindTexture(GL_TEXTURE_2D, _getRenderTargetTexture(target)->get());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);

	unsigned char* ptr = (unsigned char*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	memcpy(pixels, ptr + idxStart, size);
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

	return shared_ptr<Image>(new Image(width, height, ImageFormat::RGBA,
		InternalFormat::RGBA16, 0, boost::shared_array<unsigned char>(pixels)));
}

boost::shared_array<unsigned short> OpenGL43Graphics::getRenderImagePixel(int x, int y, RenderOptions::RenderTarget target)
{
	shared_ptr<Image> image = getRenderImage(x, y, 1, 1, target);
	return image->getPixelUS(0, 0);
}

int OpenGL43Graphics::getFrameBufferWidth()
{
	return m_gBuffer->getWidth();
}

int OpenGL43Graphics::getFrameBufferHeight()
{
	return m_gBuffer->getHeight();
}