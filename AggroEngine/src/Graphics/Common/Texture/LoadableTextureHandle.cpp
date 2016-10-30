#include "LoadableTextureHandle.hpp"

LoadableTextureHandle::LoadableTextureHandle(shared_ptr<TextureHandle> handle)
	: m_delegate(handle)
{

}

void LoadableTextureHandle::setHandle(shared_ptr<TextureHandle> handle)
{
	m_delegate = handle;
}

unsigned int LoadableTextureHandle::get()
{
	return m_delegate->get();
}