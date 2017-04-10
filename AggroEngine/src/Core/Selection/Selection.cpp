#include "Selection.hpp"
#include <algorithm>

Selection::Selection(unsigned int size)
	: m_selectedArea()
	, m_size(size)
{

}

void Selection::updateSelection(shared_ptr<MouseState> mouse, shared_ptr<Graphics> graphics)
{
	float percentX = (float)mouse->getPosX() / (float)graphics->getViewport()->getWidth();
	float percentY = (float)mouse->getPosY() / (float)graphics->getViewport()->getHeight();
	unsigned int imgX = std::max<unsigned int>(0, percentX * graphics->getFrameBufferWidth() - (m_size / 2));
	unsigned int imgY = std::max<unsigned int>(0, graphics->getFrameBufferHeight() - (percentY * graphics->getFrameBufferHeight()) - (m_size / 2));
	m_selectedArea = graphics->getRenderImage(imgX, imgY, m_size, m_size, RenderOptions::SELECTION);
}

unsigned int Selection::getSelectionAsId()
{
	return getSelectionAsId(m_size / 2, m_size / 2);
}

unsigned int Selection::getSelectionAsId(unsigned int x, unsigned int y)
{
	boost::shared_array<unsigned short> selectedPixel = getSelectionAsColor(x, y);
	unsigned short r = selectedPixel[0] / 256;
	unsigned short g = selectedPixel[1] / 256;
	unsigned short b = selectedPixel[2] / 256;
	return r + (g * 255) + (b * 65025);
}

boost::shared_array<unsigned short> Selection::getSelectionAsColor()
{
	return getSelectionAsColor(m_size / 2, m_size / 2);
}

boost::shared_array<unsigned short> Selection::getSelectionAsColor(unsigned int x, unsigned int y)
{
	return m_selectedArea->getPixelUS(std::min<unsigned int>(m_size - 1, x), std::min<unsigned int>(m_size - 1, y));
}

unsigned int Selection::getSize()
{
	return m_size;
}