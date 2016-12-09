#pragma once

#include "Texture.hpp"
#include "MathUtil.hpp"

/**
* Image of a solid color
*
* author: wcrane
* since: 2016-11-12
**/
class RGBImage : public Image
{
private:
	boost::shared_array<unsigned char> _getData(unsigned int nWidth, unsigned int nHeight, glm::vec3 color);

public:
	RGBImage(unsigned int nWidth, unsigned int nHeight, glm::vec3 color);
};