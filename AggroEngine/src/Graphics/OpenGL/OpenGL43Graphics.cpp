#include "OpenGL43Graphics.hpp"
#include "DevTexture.hpp"
#include "Grid.hpp"
#include "Config.hpp"
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
	const Properties& props = gConfig->getProperties();
	vector<int> nDimensions = props.getIntArrayProperty("graphics.resolution");
	m_gBuffer = shared_ptr<GBuffer>(new GBuffer(this, nDimensions[0], nDimensions[1]));
}

shared_ptr<VertexBufferHandle> OpenGL43Graphics::createVertexBuffer(shared_ptr<Mesh> mesh)
{
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
	glDeleteBuffers(1, &nVBOHandle);
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture()
{
	DevTexture texture(16,16);
	return texture.getHandle();
}

shared_ptr<TextureHandle> OpenGL43Graphics::createTexture(shared_ptr<TextureBuildOptions> pTexOptions)
{
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
	glDeleteTextures(1,&nHandle);
}

void OpenGL43Graphics::stageTriangleRender(shared_ptr<RenderData> pRenderData)
{ 
	renderQueue.push(pRenderData);
}

void OpenGL43Graphics::executeRender(RenderOptions &renderOptions)
{
	m_gBuffer->drawToBuffer(renderOptions, renderQueue);
	_drawScreen(renderOptions, 0.0, 0.0, 1.0, 1.0);
}

void OpenGL43Graphics::setViewport(int nX, int nY, int nWidth, int nHeight)
{
	glViewport(nX, nY, nWidth, nHeight);
}

void OpenGL43Graphics::clearColor()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepth()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGL43Graphics::clearDepthAndColor()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

ShaderStore OpenGL43Graphics::getShaderStore()
{
	return m_shaderStore;
}

void OpenGL43Graphics::_drawScreen(RenderOptions &renderOptions, float nX1, float nY1, float nX2, float nY2)
{
	clearDepth();
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
	if (renderOptions.getRenderMode() == RenderOptions::SHADED)
	{
		m_gBuffer->bindAlbedoTex();
	}
	else if (renderOptions.getRenderMode() == RenderOptions::ALBEDO)
	{
		m_gBuffer->bindAlbedoTex();
	}
	else if(renderOptions.getRenderMode() == RenderOptions::NORMAL)
	{
		m_gBuffer->bindNormalTex();
	}
	else if(renderOptions.getRenderMode() == RenderOptions::SELECTION)
	{
		m_gBuffer->bindNormalTex();
	}
	else
	{
		m_gBuffer->bindAlbedoTex();
	}
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);		 glVertex2f(nX1, nY1);
		glTexCoord2f(1.0f, 0);	 glVertex2f(nX2, nY1);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(nX2, nY2);
		glTexCoord2f(0, 1.0f);	 glVertex2f(nX1, nY2);
	glEnd();
}