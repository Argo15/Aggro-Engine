#pragma once

#include "TextureHandle.hpp"

class AsyncTextureHandle : public TextureHandle
{
private:
	unsigned int m_textureId;
	bool m_loaded;
	int m_remainingFrames;

public:
	AsyncTextureHandle(int nTextureId);

	unsigned int get();
	bool isLoaded();

	void setLoaded();
	bool isPixelDataReady();
};