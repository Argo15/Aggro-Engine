#pragma once

class TextureHandle
{
private:
	unsigned int m_nTextureId;

public:
	TextureHandle(int nTextureId);

	unsigned int get();
};