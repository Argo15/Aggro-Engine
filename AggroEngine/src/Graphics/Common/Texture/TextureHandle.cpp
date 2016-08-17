#include "TextureHandle.hpp"

TextureHandle::TextureHandle(int nTextureId)
	: m_nTextureId(nTextureId)
{

}

unsigned int TextureHandle::get()
{
	return m_nTextureId;
}