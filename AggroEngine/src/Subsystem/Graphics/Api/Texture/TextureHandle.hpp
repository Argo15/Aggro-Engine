#pragma once

class TextureHandle
{
public:
	virtual unsigned int get() = 0;
	virtual bool isLoaded() = 0;
};