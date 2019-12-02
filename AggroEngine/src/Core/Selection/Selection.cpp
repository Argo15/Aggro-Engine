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
	float percentX = (float)mouse->getPosX() / (float)graphics->getViewport()->getWidth();
	float percentY = (float)mouse->getPosY() / (float)graphics->getViewport()->getHeight();
	unsigned int imgX = std::max<unsigned int>(0, percentX * graphics->getFrameBufferWidth() - (m_size / 2));
	unsigned int imgY = std::max<unsigned int>(0, graphics->getFrameBufferHeight() - (percentY * graphics->getFrameBufferHeight()) - (m_size / 2));
	m_selectedArea = graphics->getSelectionImage(imgX, imgY, m_size, m_size);

}

unsigned int Selection::getSelectionAsId()
{
	return getSelectionAsId(m_size / 2, m_size / 2);
}

unsigned int Selection::getSelectionAsId(unsigned int x, unsigned int y)
{
	shared_ptr<unsigned char> selectedPixel = getSelectionAsColor(x, y);
	if (!selectedPixel)
	{
		return -1;
	}
	unsigned char r = selectedPixel.get()[0];
	unsigned char g = selectedPixel.get()[1];
	unsigned char b = selectedPixel.get()[2];
	return r + (g * 255) + (b * 65025);
}

shared_ptr<unsigned char> Selection::getSelectionAsColor()
{
	return getSelectionAsColor(m_size / 2, m_size / 2);
}

shared_ptr<unsigned char> Selection::getSelectionAsColor(unsigned int x, unsigned int y)
{
	if (!m_selectedArea)
	{
		return shared_ptr<unsigned char>();
	}
	return m_selectedArea->getPixel(std::min<unsigned int>(m_size - 1, x), std::min<unsigned int>(m_size - 1, y));
}

unsigned int Selection::getSize()
{
	return m_size;
}

float Selection::getDepthVal(shared_ptr<MouseState> mouse, shared_ptr<Graphics> graphics)
{
	auto tracker = PerfStats::instance().trackTime("getDepthVal");
	float percentX = (float)mouse->getPosX() / (float)graphics->getViewport()->getWidth();
	float percentY = (float)mouse->getPosY() / (float)graphics->getViewport()->getHeight();
	unsigned int depthX = std::max<unsigned int>(0, percentX * graphics->getFrameBufferWidth());
	unsigned int depthY = std::max<unsigned int>(0, graphics->getFrameBufferHeight() - (percentY * graphics->getFrameBufferHeight()));
	auto depthPixel = graphics->getDepthImagePixel(depthX, depthY);
	m_depthVal = depthPixel ? depthPixel.get()[0] : -1;
	return m_depthVal;
}