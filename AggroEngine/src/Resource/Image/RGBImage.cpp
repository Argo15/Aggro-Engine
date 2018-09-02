#include "RGBImage.hpp"

shared_ptr<unsigned char> RGBImage::_getData(unsigned int nWidth, unsigned int nHeight, glm::vec3 color)
{
	nWidth = roundUpToNextPowerOfTwo(nWidth); // we want a pow2 texture
	nHeight = roundUpToNextPowerOfTwo(nHeight);
	int nTextureSize = nWidth * nHeight * 3;
	unsigned char *pTexture = new unsigned char[nTextureSize]; // Array to hold texture RGB values 
	for (int i = 0; i<nTextureSize; i += 3)       //Set color value to temporary array
	{
		pTexture[i + 0] = (unsigned char)255 * color[0];
		pTexture[i + 1] = (unsigned char)255 * color[1];
		pTexture[i + 2] = (unsigned char)255 * color[2];
	}
	return mem::shared_array<unsigned char>(pTexture, nTextureSize, "Image");
}

RGBImage::RGBImage(unsigned int nWidth, unsigned int nHeight, glm::vec3 color)
	: Image(nWidth, nHeight, _getData(nWidth, nHeight, color))
{
}