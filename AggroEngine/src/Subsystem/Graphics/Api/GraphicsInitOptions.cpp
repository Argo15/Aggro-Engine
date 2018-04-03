#include "GraphicsInitOptions.hpp"
#include "Config.hpp"

GraphicsInitOptions::GraphicsInitOptions()
{
	const Properties& props = Config::instance().getProperties();
	vector<int> nDimensions = props.getIntArrayProperty("graphics.resolution");
	m_bufferWidth = nDimensions[0];
	m_bufferHeight = nDimensions[1];
	m_shadowSize = 2048;
}

GraphicsInitOptions *GraphicsInitOptions::setBufferDimensions(int width, int height)
{
	m_bufferWidth = width;
	m_bufferHeight = height;
	return this;
}

GraphicsInitOptions *GraphicsInitOptions::setShadowSize(int dim)
{
	m_shadowSize = dim;
	return this;
}

int GraphicsInitOptions::getBufferWidth()
{
	return m_bufferWidth;
}

int GraphicsInitOptions::getBufferHeight()
{
	return m_bufferHeight;
}

int GraphicsInitOptions::getShadowSize()
{
	return m_shadowSize;
}