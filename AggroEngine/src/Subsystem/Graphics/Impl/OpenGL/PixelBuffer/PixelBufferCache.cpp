#include "PixelBufferCache.hpp"
#include "StringUtil.hpp"

PixelBufferCache::PixelBufferCache(shared_ptr<GraphicsInitOptions> options)
	: m_curIndex(0)
	, m_numBuffs(3)
	, m_width(options->getBufferWidth())
	, m_height(options->getBufferHeight())
{
	m_selectionPBOs = new GLuint[m_numBuffs];
	glGenBuffers(m_numBuffs, m_selectionPBOs);
	for (int i = 0; i < m_numBuffs; i++)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, m_selectionPBOs[i]);
		// Selection buffer is RGBA8, so 64 bit or 4 bytes per pixel
		glBufferData(GL_PIXEL_PACK_BUFFER, m_width * m_height * 4, 0, GL_STREAM_READ);
	}

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void PixelBufferCache::writeSelectionBuffer(shared_ptr<GBuffer> gBuffer)
{
	int nextIndex = (m_curIndex + 1) % m_numBuffs;
	gBuffer->bindFrameBufferReadOnly();

	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_selectionPBOs[m_curIndex]);
	glReadBuffer(gBuffer->getSelectionColorAttachment());
	glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	gBuffer->unbindFrameBufferReadOnly();
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	
	m_curIndex = nextIndex;
}

shared_ptr<ImageUC> PixelBufferCache::getSelectionImage(int x, int y, int width, int height)
{
	x = min(max(x, 0), m_width - 1);
	y = min(max(y, 0), m_height - 1);
	width = min(width, m_width - x - 1);
	height = min(height, m_height - y - 1);

	// 4 bytes per pixel
	int offset = 4 * (m_width * y + x);
    /* 
		To draw a picture, if x = 1 and y = 2 and we need 3 by 3, 
		then we require a length of: pbo_w * (h-1) + w
		* * * * * * * *
		x o o o * * * *
		x o o o x x x x
		* o o o x x x x
		* * * * * * * *
		* * * * * * * *
	*/
	int length = 4 * (m_width * (height - 1) + width);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_selectionPBOs[m_curIndex]);
	GLubyte* pixels = (GLubyte*)glMapBufferRange(GL_PIXEL_PACK_BUFFER, offset, length, GL_MAP_READ_BIT);
	if (pixels)
	{
		int numBytes = 4 * height * width;
		unsigned char *selectionPixels = new unsigned char[numBytes];
		for (int i = 0; i < height; i++)
		{
			memcpy(selectionPixels + i * 4 * width, pixels + i * 4 * m_width, 4 * width);
		}
		shared_ptr<ImageUC> selectionImage = shared_ptr<ImageUC>(
			(new ImageUC(width, height, mem::shared_array(selectionPixels, numBytes, "GL_Graphics")))
			->setImageFormat(ImageFormat::RGBA)
			->setImageType(ImageType::UNSIGNED_BYTE)
			);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		return selectionImage;
	}
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	return shared_ptr<ImageUC>();
}