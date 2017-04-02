#include "DevTexture.hpp"
#include <iostream>

DevTexture::DevTexture(unsigned int nWidth, unsigned int nHeight)
{
	nWidth = roundUpToNextPowerOfTwo(nWidth); // we want a pow2 texture
	nHeight = roundUpToNextPowerOfTwo(nHeight);
	int nTextureSize = nWidth*nHeight*3;
	unsigned char *pTexture = new unsigned char[nTextureSize]; //Temporary array to hold texture RGB values 

	for(int i=0; i<nTextureSize; i+=3)       //Set color value to temporary array
	{
		int ypos = i / 3 / nWidth;
		if ((i+ypos%2) % 2 == 0)
		{
			pTexture[i+0]=(unsigned char)0;
			pTexture[i+1]=(unsigned char)(80+((float)i/(float)nTextureSize*150));
			pTexture[i+2]=(unsigned char)(150 + ((float)i / (float)nTextureSize * 80));
		}
		else
		{
			pTexture[i+0]=(unsigned char)128;
			pTexture[i+1]=(unsigned char)128;
			pTexture[i+2]=(unsigned char)128;
		}
	}
	
	glGenTextures(1, &m_nHandle);
	glBindTexture(GL_TEXTURE_2D, m_nHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, nWidth, nHeight, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	delete[] pTexture;
}