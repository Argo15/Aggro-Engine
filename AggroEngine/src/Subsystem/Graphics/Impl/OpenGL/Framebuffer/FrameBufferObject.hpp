#pragma once

#include "Shaders/GLSLProgram.hpp"
#include "TextureHandle.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class FrameBufferObject
{
protected:
	shared_ptr<GLSLProgram> m_glslProgram;
	shared_ptr<TextureHandle> m_texture;
	GLuint m_buffer;

	int m_width;
	int m_height;

public:
	FrameBufferObject(int width, int height);

	void bindFrameBuffer();
	void unbindFrameBuffer();

	void bindTexture();

	shared_ptr<TextureHandle> getTexture();
	shared_ptr<GLSLProgram> getProgram();
	GLuint getBuffer();

	int getWidth();
	int getHeight();
};