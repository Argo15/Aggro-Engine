#include "OpenGL43Graphics.hpp"
#include "Textures/WhiteTexture.hpp"
#include "DefaultTextureHandle.hpp"
#include "DefaultVertexBufferHandle.hpp"
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
	this->lock();
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->unlock();
	const Properties& props = gConfig->getProperties();
	vector<int> nDimensions = props.getIntArrayProperty("graphics.resolution");
	m_shadowBuffer = shared_ptr<ShadowMapBuffer>(new ShadowMapBuffer(this, 2048));
	m_gBuffer = shared_ptr<GBuffer>(new GBuffer(this, nDimensions[0], nDimensions[1]));
	m_lightBuffer = shared_ptr<LightBuffer>(new LightBuffer(this, nDimensions[0], nDimensions[1]));
	m_shadedBuffer = shared_ptr<ShadedBuffer>(new ShadedBuffer(this, nDimensions[0], nDimensions[1]));
	m_viewport = shared_ptr<Viewport>(new Viewport());
}

shared_ptr<VertexBufferHandle> OpenGL43Graphics::createVertexBuffer(shared_ptr<Mesh> mesh)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
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
	return shared_ptr<VertexBufferHandle>(new DefaultVertexBufferHandle(nVertexHandle, mesh->getSizeOfVerticies(), nIndexHandle, mesh->getSizeOfIndicies()));
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
	WhiteTexture texture(1,1);
	return texture.getHandle();
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture(shared_ptr<Image> image)
{
	return createTexture(shared_ptr<TextureBuildOptions>(new TextureBuildOptions(image)));
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture(shared_ptr<TextureBuildOptions> pTexOptions)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
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

void OpenGL43Graphics::stageTriangleRender(shared_ptr<RenderData> pRenderData)
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	renderQueue.push(pRenderData);
}

void OpenGL43Graphics::executeRender(RenderOptions &renderOptions)
{
	m_shadowBuffer->drawToBuffer(renderOptions, renderQueue);
	m_gBuffer->drawToBuffer(renderOptions, renderQueue);
	m_lightBuffer->drawToBuffer(renderOptions, m_gBuffer->getNormalTex(), m_gBuffer->getDepthTex(), m_gBuffer->getGlowTex(), m_shadowBuffer);
	m_shadedBuffer->drawToBuffer(renderOptions, m_gBuffer->getAlbedoTex(), m_lightBuffer->getTexture(), m_lightBuffer->getGlowTex());
	_drawScreen(renderOptions, 0.0, 0.0, 1.0, 1.0);
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

void OpenGL43Graphics::clearColor()
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glClearColor(0.3f, 0.3f, 0.3f, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepth()
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepthAndColor()
{
	boost::lock_guard<OpenGL43Graphics> guard(*this);
	glClearColor(0.3f, 0.3f, 0.3f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

ShaderStore OpenGL43Graphics::getShaderStore()
{
	return m_shaderStore;
}

void OpenGL43Graphics::_drawScreen(RenderOptions &renderOptions, float nX1, float nY1, float nX2, float nY2)
{
	clearDepth();
	boost::lock_guard<OpenGL43Graphics> guard(*this);

	glm::vec4 viewport = renderOptions.getViewport();
	setViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

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
	boost::lock_guard<OpenGL43Graphics> guard(*this);

	int pixelSize = sizeof(unsigned short) * 4;
	int size = width * height * pixelSize;
	unsigned char *pixels = new unsigned char[size];

	m_gBuffer->bindFrameBuffer();
	glReadBuffer(m_gBuffer->getSelectionColorAttachment());
	glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_SHORT, pixels);
	m_gBuffer->unbindFrameBuffer();

	return shared_ptr<Image>((new Image(width, height, boost::shared_array<unsigned char>(pixels)))
		->setImageFormat(ImageFormat::RGBA)
		->setImageType(ImageType::UNSIGNED_SHORT)
	);
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