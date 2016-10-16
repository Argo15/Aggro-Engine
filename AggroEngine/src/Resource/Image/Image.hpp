#pragma once

#include <memory>
#include <boost/shared_array.hpp>
using namespace std;

// Mapped to the same values as OpenGL
enum ImageFormat
{
	RGB = 0x1907,
	RGBA = 0x1908,
	BGRA = 0x80E1,
	DEPTH_COMPONENT = 0x1902,
	RED = 0x1903
};
enum InternalFormat
{
	RGBA8 = 0x8058,
	RGBA16 = 0x805B,
	DEPTH_COMPONENT32 = 0x81A7,
	R16UI = 0x822A
};
enum ImageType
{
	UNSIGNED_BYTE = 0x1401,
	FLOAT_TYPE = 0x1406, 
	INT_TYPE = 0x1404,
	UNSIGNED_INT_TYPE = 0x1405
};

/**
 * Holds image data
 *
 * author: wcrane
 * since: 2014-04-12
**/
class Image
{
private:
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	ImageFormat m_format;
	InternalFormat m_internalFormat;
	ImageType m_type;
	unsigned int m_nComponents;

	boost::shared_array<unsigned char> m_npData;

public:
	Image(unsigned int nWidth, unsigned int nHeight, boost::shared_array<unsigned char> npData);
	Image(unsigned int nWidth, unsigned int nHeight, ImageFormat format, InternalFormat internalFormat, unsigned int nComponents, boost::shared_array<unsigned char> npData);
	
	Image *setImageFormat(ImageFormat format);
	Image *setInternalFormat(InternalFormat format);
	Image *setImageType(ImageType type);

	unsigned int getWidth();
	unsigned int getHeight();
	ImageFormat getFormat();
	InternalFormat getInternalFormat();
	ImageType getImageType();
	unsigned int getComponents();
	boost::shared_array<unsigned char> getData();
	boost::shared_array<float> getPixelF(int x, int y);
	boost::shared_array<unsigned short> Image::getPixelUS(int x, int y);
};