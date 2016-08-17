#include "FreeImageImporter.hpp"
#include "CheckersImage.hpp"
#include "FreeImage.h"
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

shared_ptr<Image> FreeImageImporter::importImage(string sFilename)
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
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		imagen = FreeImage_Load(fif, sFilename.c_str());
	//if the image failed to load, return failure
	if(!imagen)
		return shared_ptr<Image>(new CheckersImage(16,16));

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);
	
	//retrieve the image data
	bits = FreeImage_GetBits(imagen);
	//get the image width and height
	width = FreeImage_GetWidth(imagen);
	height = FreeImage_GetHeight(imagen);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return shared_ptr<Image>(new CheckersImage(16,16));

	
	//return shared_ptr<Image>(new CheckersImage(16,16));
	//return success
	return shared_ptr<Image>(new Image(width,height,ImageFormat::BGRA, 4,boost::shared_array<unsigned char>(bits, [imagen](unsigned char* a){ FreeImage_Unload(imagen); })));
}