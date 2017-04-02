#include "Texture.hpp"
#include "DefaultTextureHandle.hpp"

Texture::Texture()
	: m_nHandle(0)
{

}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_nHandle);
}

void Texture::remove()
{
	glDeleteTextures(1,&m_nHandle);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

shared_ptr<TextureHandle> Texture::getHandle()
{
	return shared_ptr<TextureHandle>(new DefaultTextureHandle(m_nHandle));
}