#include "Image.hpp"

Image::Image(unsigned int nWidth, unsigned int nHeight, boost::shared_array<unsigned char> npData)
	: m_nWidth(nWidth)
	, m_nHeight(nHeight)
	, m_format(RGB)
	, m_internalFormat(RGBA8)
	, m_type(UNSIGNED_BYTE)
	, m_nComponents(3)
	, m_npData(npData)
{

}

Image::Image(unsigned int nWidth, unsigned int nHeight, ImageFormat format, InternalFormat internalFormat, unsigned int nComponents, boost::shared_array<unsigned char> npData)
	: m_nWidth(nWidth)
	, m_nHeight(nHeight)
	, m_format(format)
	, m_internalFormat(internalFormat)
	, m_type(UNSIGNED_BYTE)
	, m_nComponents(nComponents)
	, m_npData(npData)
{

}

Image *Image::setImageFormat(ImageFormat format)
{
	m_format = format;
	return this;
}

Image *Image::setInternalFormat(InternalFormat format)
{
	m_internalFormat = format;
	return this;
}

Image *Image::setImageType(ImageType type)
{
	m_type = type;
	return this;
}

unsigned int Image::getWidth()
{
	return m_nWidth;
}
	
unsigned int Image::getHeight()
{
	return m_nHeight;
}

ImageFormat Image::getFormat()
{
	return m_format;
}

InternalFormat Image::getInternalFormat()
{
	return m_internalFormat;
}

ImageType Image::getImageType()
{
	return m_type;
}

unsigned int Image::getComponents()
{
	return m_nComponents;
}

boost::shared_array<unsigned char> Image::getData()
{
	return m_npData;
}