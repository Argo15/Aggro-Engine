#include "Selection.hpp"
#include "PerfStats.hpp"
#include <algorithm>

Selection::Selection(unsigned int size)
	: m_selectedArea()
	, m_size(size)
{

}

void Selection::updateSelection(shared_ptr<MouseState> mouse, shared_ptr<Graphics> graphics)
{
	auto tracker = PerfStats::instance().trackTime("updateSelection");
	float percentX = (float)mouse->getPosX() / (float)graphics->getViewport()->getWidth();
	float percentY = (float)mouse->getPosY() / (float)graphics->getViewport()->getHeight();
	unsigned int imgX = std::max<unsigned int>(0, percentX * graphics->getFrameBufferWidth() - (m_size / 2));
	unsigned int imgY = std::max<unsigned int>(0, graphics->getFrameBufferHeight() - (percentY * graphics->getFrameBufferHeight()) - (m_size / 2));
	m_selectedArea = graphics->getRenderImage(imgX, imgY, m_size, m_size, RenderOptions::SELECTION);

	unsigned int depthX = std::max<unsigned int>(0, percentX * graphics->getFrameBufferWidth());
	unsigned int depthY = std::max<unsigned int>(0, graphics->getFrameBufferHeight() - (percentY * graphics->getFrameBufferHeight()));
	auto depthPixel = graphics->getRenderImagePixelF(depthX, depthY, RenderOptions::DEPTH);
	m_depthVal = depthPixel ? depthPixel.get()[0] : -1;
}

unsigned int Selection::getSelectionAsId()
{
	return getSelectionAsId(m_size / 2, m_size / 2);
}

unsigned int Selection::getSelectionAsId(unsigned int x, unsigned int y)
{
	shared_ptr<unsigned short> selectedPixel = getSelectionAsColor(x, y);
	if (!selectedPixel)
	{
		return -1;
	}
	unsigned short r = selectedPixel.get()[0] / 256;
	unsigned short g = selectedPixel.get()[1] / 256;
	unsigned short b = selectedPixel.get()[2] / 256;
	return r + (g * 255) + (b * 65025);
}

shared_ptr<unsigned short> Selection::getSelectionAsColor()
{
	return getSelectionAsColor(m_size / 2, m_size / 2);
}

shared_ptr<unsigned short> Selection::getSelectionAsColor(unsigned int x, unsigned int y)
{
	if (!m_selectedArea)
	{
		return shared_ptr<unsigned short>();
	}
	return m_selectedArea->getPixel(std::min<unsigned int>(m_size - 1, x), std::min<unsigned int>(m_size - 1, y));
}

unsigned int Selection::getSize()
{
	return m_size;
}

float Selection::getDepthVal()
{
	return m_depthVal;
}