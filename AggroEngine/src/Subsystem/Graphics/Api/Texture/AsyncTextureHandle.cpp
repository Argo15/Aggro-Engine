#include "AsyncTextureHandle.hpp"

AsyncTextureHandle::AsyncTextureHandle(int nTextureId)
	: m_textureId(nTextureId)
	, m_loaded(false)
	, m_remainingFrames(3)
{

}

unsigned int AsyncTextureHandle::get()
{
	return m_textureId;
}

bool AsyncTextureHandle::isLoaded()
{
	return m_loaded;
}

void AsyncTextureHandle::setLoaded()
{
	m_loaded = true;
}

bool AsyncTextureHandle::isPixelDataReady()
{
	if (m_remainingFrames > 0)
	{
		m_remainingFrames--;
	}
	return m_remainingFrames == 0;
}