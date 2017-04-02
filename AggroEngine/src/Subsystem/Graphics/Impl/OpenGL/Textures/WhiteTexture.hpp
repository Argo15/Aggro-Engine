#pragma once

#include "Texture.hpp"
#include "MathUtil.hpp"

/**
* Pure white texture. Used as a default.
*
* author: wcrane
* since: 2016-10-28
**/
class WhiteTexture : public Texture
{
public:
	WhiteTexture(unsigned int nWidth, unsigned int nHeight, unsigned int shade = 128);
};