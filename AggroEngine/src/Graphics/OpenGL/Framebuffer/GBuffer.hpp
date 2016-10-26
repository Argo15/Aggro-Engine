#pragma once

class GBuffer;

#include "OpenGL43Graphics.hpp"

class GBuffer
{
private:
	shared_ptr<GLSLProgram> m_gBufferProg;
	shared_ptr<TextureHandle> m_depthTex;
	shared_ptr<TextureHandle> m_normalTex;
	shared_ptr<TextureHandle> m_albedoTex;
	shared_ptr<TextureHandle> m_selectionTex;
	GLuint m_buffer;
	OpenGL43Graphics *m_graphics;

	int m_width;
	int m_height;

public:
	GBuffer(OpenGL43Graphics *graphics, int width, int height);

	void drawToBuffer(RenderOptions renderOptions, std::queue<shared_ptr<RenderData>> &renderQueue);

	void bind();
	void unbind();

	void bindDepthTex();
	void bindNormalTex();
	void bindAlbedoTex();
	void bindSelectionTex();

	shared_ptr<TextureHandle> getDepthTex();
	shared_ptr<TextureHandle> getNormalTex();
	shared_ptr<TextureHandle> getAlbedoTex();
	shared_ptr<TextureHandle> getSelectionTex();

	shared_ptr<GLSLProgram> getProgram();

	int getWidth();
	int getHeight();
};