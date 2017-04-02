#pragma once

#include "OpenGL43Graphics.hpp"

class FrameBufferObject
{
protected:
	GLuint m_texture;
	GLuint m_buffer;

public:
	FrameBufferObject();
	void bind();
	void unbind();
	GLuint getBuffer();
	GLuint getTexture();
};