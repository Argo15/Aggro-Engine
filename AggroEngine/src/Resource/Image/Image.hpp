#pragma once

#include <memory>
#include "MemoryUtil.hpp"
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
 * Holds image data. 
 * T = component size. RGBA16 would be unsigned short, RGBA8 is char.
 * A depth texture has one float component, so T = float
 *
 * author: wcrane
 * since: 2014-04-12
**/
template <class T>
class Image
{
private:
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	ImageFormat m_format;
	ImageType m_type;

	shared_ptr<T> m_npData;

public:
	Image(unsigned int nWidth, unsigned int nHeight, shared_ptr<T> npData)
		: m_nWidth(nWidth)
		, m_nHeight(nHeight)
		, m_format(RGB)
		, m_type(UNSIGNED_BYTE)
		, m_npData(npData)
	{}

	~Image()
	{}

	Image *setImageFormat(ImageFormat format)
	{
		m_format = format;
		return this;
	}

	Image *setImageType(ImageType type)
	{
		m_type = type;
		return this;
	}

	unsigned int getWidth()
	{
		return m_nWidth;
	}

	unsigned int getHeight()
	{
		return m_nHeight;
	}

	ImageFormat getFormat()
	{
		return m_format;
	}

	ImageType getImageType()
	{
		return m_type;
	}

	shared_ptr<T> getData()
	{
		return m_npData;
	}

	shared_ptr<T> getPixel(int x, int y)
	{
		if (x >= m_nWidth || y >= m_nHeight)
		{
			return shared_ptr<T>();
		}

		int numComponents = getNumComponents();
		int pixelSize = sizeof(T) * numComponents;
		int idxStart = (y * m_nWidth * numComponents) + (x * numComponents);

		T *pixel = new T[numComponents];
		memcpy(pixel, m_npData.get() + idxStart, pixelSize);

		shared_ptr<T> ret = mem::shared_array<T>(pixel, numComponents, "Image");

		return ret;
	}

	int getSize()
	{
		return m_nWidth * m_nHeight * sizeof(T) * getNumComponents();
	}

	int getNumComponents()
	{
		switch (m_format)
		{
		case DEPTH_COMPONENT: return 1;
		case RGB: return 3;
		default: return 4;
		}
	}
};

using ImageUC = Image<unsigned char>;
using ImageUS = Image<unsigned short>;
using ImageF = Image<float>;