#include "PixelBufferCache.hpp"
#include "StringUtil.hpp"

PixelBufferCache::PixelBufferCache()
{

}

GLuint PixelBufferCache::getPixelBuffer(int size, string ns)
{
	string key = ns + "." + StringUtil::intToString(size);
	if (m_sizeToPBO.find(key) == m_sizeToPBO.end())
	{
		GLuint pbo;
		glGenBuffers(1, &pbo);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
		glBufferData(GL_PIXEL_PACK_BUFFER, size, 0, GL_STREAM_READ);
		m_sizeToPBO[key] = pbo;
	}

	return m_sizeToPBO[key];
}