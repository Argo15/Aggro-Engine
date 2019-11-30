#include "LoadableTextureHandle.hpp"
#include <iostream>

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
	if (!isLoaded())
	{
		cout << "Warning, calling LoadableTextureHandle.get() on unloaded texture." << endl;
		return -1;
	}
	return m_delegate->get();
}

bool LoadableTextureHandle::isLoaded()
{
	return m_delegate != nullptr && m_delegate->isLoaded();
}