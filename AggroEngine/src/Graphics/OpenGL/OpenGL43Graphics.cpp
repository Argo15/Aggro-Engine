#include "OpenGL43Graphics.hpp"
#include "../../Graphics/OpenGL/Textures/DevTexture.hpp"
#include "../Resource/Mesh/Dev/Grid.hpp"
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

unique_ptr<DevTexture> devTexture;

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
	devTexture = unique_ptr<DevTexture>(new DevTexture(32, 32));
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
	glBindTexture(GL_TEXTURE_2D, m_nHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->getComponents(), pImage->getWidth(), pImage->getHeight(), pImage->getFormat(), GL_UNSIGNED_BYTE, pImage->getData().get());
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

void OpenGL43Graphics::executeRender(RenderOptions renderOptions)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(glm::value_ptr(renderOptions.getProjectionMatrix()));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(glm::value_ptr(renderOptions.getViewMatrix()));

	shared_ptr<GLSLProgram> glslProgram = m_shaderStore.getShader("Resources/Shaders/v_devtest.glsl", "Resources/Shaders/f_devtest.glsl");

	glslProgram->use();

	while (!renderQueue.empty())
	{
		shared_ptr<RenderData> renderData = renderQueue.front();
		renderQueue.pop();

		if (renderData->getVertexBufferHandle() != nullptr)
		{
			glm::mat4 mvpMatrix = renderOptions.getProjectionMatrix()*renderOptions.getViewMatrix()*renderData->getModelMatrix();
			glslProgram->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvpMatrix), false, 4);
			glm::mat3 normalMatrix = glm::mat3(renderData->getModelMatrix());
			glslProgram->sendUniform("normalMatrix", glm::value_ptr(normalMatrix), false, 3);

			glActiveTexture(GL_TEXTURE0);
			if (renderData->getTextureHandle().get() != nullptr)
			{
				glBindTexture(GL_TEXTURE_2D, renderData->getTextureHandle()->get());
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, devTexture->getHandle()->get());
			}
			glslProgram->sendUniform("tex", 0);

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

	glslProgram->disable();
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