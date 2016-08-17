#include "CheckersImage.hpp"

boost::shared_array<unsigned char> CheckersImage::getData(int nWidth, int nHeight)
{
	nWidth = roundUpToNextPowerOfTwo(nWidth); // we want a pow2 texture
	nHeight = roundUpToNextPowerOfTwo(nHeight);
	int nTextureSize = nWidth*nHeight*3;
	unsigned char *pTexture = new unsigned char[nTextureSize]; // Array to hold texture RGB values 

	for(int i=0; i<nTextureSize; i+=3)       //Set color values to array
	{
		int ypos = i / 3 / nWidth;
		if ((i+ypos%2) % 2 == 0)
		{
			pTexture[i+0]=(unsigned char)0;
			pTexture[i+1]=(unsigned char)80;
			pTexture[i+2]=(unsigned char)200;
		}
		else
		{
			pTexture[i+0]=(unsigned char)128;
			pTexture[i+1]=(unsigned char)128;
			pTexture[i+2]=(unsigned char)128;
		}
	}
	return boost::shared_array<unsigned char>(pTexture);
}

CheckersImage::CheckersImage(int nWidth, int nHeight) 
	: Image(nWidth, nHeight, getData(nWidth, nHeight))
{

}