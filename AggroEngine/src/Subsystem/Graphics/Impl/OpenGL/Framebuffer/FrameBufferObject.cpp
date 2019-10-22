#include "FrameBufferObject.hpp"

FrameBufferObject::FrameBufferObject(int width, int height)
{
	m_width = width;
	m_height = height;
}

void FrameBufferObject::bindFrameBufferRW()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
}

void FrameBufferObject::bindFrameBufferReadOnly()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_buffer);
}

void FrameBufferObject::bindFrameBufferWriteOnly()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_buffer);
}

void FrameBufferObject::unbindFrameBufferRW()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::unbindFrameBufferReadOnly()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBufferObject::unbindFrameBufferWriteOnly()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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