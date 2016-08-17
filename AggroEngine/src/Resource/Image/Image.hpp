#pragma once

#include <memory>
#include <boost/shared_array.hpp>
using namespace std;

// Mapped to the same values as OpenGL
enum ImageFormat
{
	RGB = 0x1907,
	RGBA = 0x1908,
	BGRA = 0x80E1
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
	unsigned int m_format;
	unsigned int m_nComponents;

	boost::shared_array<unsigned char> m_npData;

public:
	Image(unsigned int nWidth, unsigned int nHeight, boost::shared_array<unsigned char> npData);
	Image(unsigned int nWidth, unsigned int nHeight, unsigned int format, unsigned int nComponents, boost::shared_array<unsigned char> npData);
	
	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getFormat();
	unsigned int getComponents();
	boost::shared_array<unsigned char> getData();
};