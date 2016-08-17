#include "Image.hpp"

Image::Image(unsigned int nWidth, unsigned int nHeight, boost::shared_array<unsigned char> npData)
	: m_nWidth(nWidth)
	, m_nHeight(nHeight)
	, m_format(RGB)
	, m_nComponents(3)
	, m_npData(npData)
{

}

Image::Image(unsigned int nWidth, unsigned int nHeight, unsigned int format, unsigned int nComponents, boost::shared_array<unsigned char> npData)
	: m_nWidth(nWidth)
	, m_nHeight(nHeight)
	, m_format(format)
	, m_nComponents(nComponents)
	, m_npData(npData)
{

}

unsigned int Image::getWidth()
{
	return m_nWidth;
}
	
unsigned int Image::getHeight()
{
	return m_nHeight;
}

unsigned int Image::getFormat()
{
	return m_format;
}

unsigned int Image::getComponents()
{
	return m_nComponents;
}

boost::shared_array<unsigned char> Image::getData()
{
	return m_npData;
}