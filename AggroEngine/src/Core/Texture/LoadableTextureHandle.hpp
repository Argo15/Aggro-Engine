#pragma once

#include <memory>
#include "TextureHandle.hpp"
using namespace std;

class LoadableTextureHandle : public TextureHandle
{
private:
	shared_ptr<TextureHandle> m_delegate;

public:
	LoadableTextureHandle(shared_ptr<TextureHandle> handle);

	void setHandle(shared_ptr<TextureHandle> handle);
	unsigned int get();
};