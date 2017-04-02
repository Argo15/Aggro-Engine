#pragma once

#include <memory>
#include <boost/shared_array.hpp>
using namespace std;

// Format values mapped to the same values as OpenGL

/**
 * Specifies the format of the pixel data.
 */
enum ImageFormat
{
	RGB = 0x1907,
	RGBA = 0x1908,
	BGRA = 0x80E1,
	DEPTH_COMPONENT = 0x1902,
	RED = 0x1903
};

/**
 * Specifies the data type of the pixel data.
 * Examples:
 * ImageFormat::RGB with ImageType::UNSIGNED_BYTE_3_3_2 is an image with 1 byte per pixel: 3 bits red, 3 bits green, and 2 bits blue
 * ImageFormat::RED with ImageType::INT_TYPE is an image with 4 bytes per pixel: all representing red
 */
enum ImageType
{
	UNSIGNED_BYTE = 0x1401,
	UNSIGNED_SHORT = 0x1403,
	INT_TYPE = 0x1404,
	UNSIGNED_INT_TYPE = 0x1405,
	FLOAT_TYPE = 0x1406,
	UNSIGNED_BYTE_3_3_2 = 0x8032
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
	ImageType m_type;

	boost::shared_array<unsigned char> m_npData;

public:
	Image(unsigned int nWidth, unsigned int nHeight, boost::shared_array<unsigned char> npData);
	
	Image *setImageFormat(ImageFormat format);
	Image *setImageType(ImageType type);

	unsigned int getWidth();
	unsigned int getHeight();
	ImageFormat getFormat();
	ImageType getImageType();
	boost::shared_array<unsigned char> getData();
	boost::shared_array<float> getPixelF(int x, int y);
	boost::shared_array<unsigned short> Image::getPixelUS(int x, int y);
};