#include "FrameBufferObject.hpp"

FrameBufferObject::FrameBufferObject(int width, int height)
{
	m_width = width;
	m_height = height;
}

void FrameBufferObject::bindFrameBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_buffer);
}

void FrameBufferObject::unbindFrameBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FrameBufferObject::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_texture->get());
}

shared_ptr<TextureHandle> FrameBufferObject::getTexture()
{
	return m_texture;
}

shared_ptr<GLSLProgram> FrameBufferObject::getProgram()
{
	return m_glslProgram;
}

GLuint FrameBufferObject::getBuffer()
{
	return m_buffer;
}

int FrameBufferObject::getWidth()
{
	return m_width;
}

int FrameBufferObject::getHeight()
{
	return m_height;
}