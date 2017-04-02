#include "Image.hpp"

Image::Image(unsigned int nWidth, unsigned int nHeight, boost::shared_array<unsigned char> npData)
	: m_nWidth(nWidth)
	, m_nHeight(nHeight)
	, m_format(RGB)
	, m_type(UNSIGNED_BYTE)
	, m_npData(npData)
{

}

Image *Image::setImageFormat(ImageFormat format)
{
	m_format = format;
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

ImageType Image::getImageType()
{
	return m_type;
}

boost::shared_array<unsigned char> Image::getData()
{
	return m_npData;
}

/**
 * Get array of pixel values, assuming each color component is 4 bytes each.
 */
boost::shared_array<float> Image::getPixelF(int x, int y)
{
	int colorSize = sizeof(float);
	int pixelSize;
	switch (m_format)
	{
		case DEPTH_COMPONENT: pixelSize = colorSize * 1;
		case RGB: pixelSize = colorSize * 3;
		default: pixelSize = colorSize * 4;
	}
	int idxStart = (y * m_nWidth * pixelSize) + (x * pixelSize);

	unsigned char *bytes = m_npData.get();
	float *pixelF = new float[4];
	memcpy(pixelF, bytes + idxStart, pixelSize);

	return boost::shared_array<float>(pixelF);
}

/**
* Get array of pixel values, assuming each color component is 2 bytes each.
*/
boost::shared_array<unsigned short> Image::getPixelUS(int x, int y)
{
	int colorSize = sizeof(unsigned short);
	int pixelSize;
	switch (m_format)
	{
	case DEPTH_COMPONENT: pixelSize = colorSize * 1;
	case RGB: pixelSize = colorSize * 3;
	default: pixelSize = colorSize * 4;
	}
	int idxStart = (y * m_nWidth * pixelSize) + (x * pixelSize);

	unsigned char *bytes = m_npData.get();
	unsigned short *pixelUS = new unsigned short[4];
	memcpy(pixelUS, bytes + idxStart, pixelSize);

	return boost::shared_array<unsigned short>(pixelUS);
}