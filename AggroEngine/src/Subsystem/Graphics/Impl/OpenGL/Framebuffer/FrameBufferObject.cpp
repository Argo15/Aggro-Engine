#include "FrameBufferObject.hpp"

FrameBufferObject::FrameBufferObject() 
{
}

void FrameBufferObject::bind()
{ 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_buffer);
}

void FrameBufferObject::unbind()
{ 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
}

GLuint FrameBufferObject::getBuffer()
{ 
	return m_buffer;
}

GLuint FrameBufferObject::getTexture()
{ 
	return m_texture;
}