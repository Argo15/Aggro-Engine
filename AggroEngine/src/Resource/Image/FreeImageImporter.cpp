#include "FreeImageImporter.hpp"
#include "CheckersImage.hpp"
#include "FreeImage.h"
#include "MemoryUtil.hpp"
#include <iostream>
using namespace std;

template< typename T >
struct freeimage_deleter
{
  void operator ()( T const * p)
  { 
    delete[] p; 
  }
};

FreeImageImporter::FreeImageImporter()
{
	FreeImage_Initialise();
}

shared_ptr<ImageUC> FreeImageImporter::importImage(string sFilename)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *imagen(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(sFilename.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(sFilename.c_str());
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
	{
		cout << "failed loading image " << sFilename << endl;
		return shared_ptr<ImageUC>(new CheckersImage(16, 16));
	}

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		imagen = FreeImage_Load(fif, sFilename.c_str());
	//if the image failed to load, return failure
	if (!imagen)
	{
		cout << "failed loading image " << sFilename << endl;
		return shared_ptr<ImageUC>(new CheckersImage(16, 16));
	}

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);
	
	//retrieve the image data
	bits = FreeImage_GetBits(imagen);
	//get the image width and height
	width = FreeImage_GetWidth(imagen);
	height = FreeImage_GetHeight(imagen);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
	{
		cout << "failed loading image " << sFilename << endl;
		return shared_ptr<ImageUC>(new CheckersImage(16, 16));
	}

	// Swap R & B channels (default is BGRA)
	for (int i = 0; i < width*height; i++)
	{
		BYTE bVal = bits[i * 4];
		BYTE rVal = bits[i * 4 + 2];
		bits[i * 4] = rVal;
		bits[i * 4 + 2] = bVal;
	}

	return shared_ptr<ImageUC>((
		new ImageUC(
			width, height, 
			shared_ptr<unsigned char>(bits, [imagen](unsigned char* a)
			{ 
				FreeImage_Unload(imagen);
			}))
		)->setImageFormat(ImageFormat::RGBA)
	);
}