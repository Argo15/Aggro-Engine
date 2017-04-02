#pragma once

#include "TextureHandle.hpp"

class DefaultTextureHandle : public TextureHandle
{
private:
	unsigned int m_nTextureId;

public:
	DefaultTextureHandle(int nTextureId);

	unsigned int get();
};