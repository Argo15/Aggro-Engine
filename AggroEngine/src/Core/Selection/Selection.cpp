#include "Selection.hpp"

Selection::Selection()
	: m_selectedPixel()
{

}

void Selection::updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context)
{
	float percentX = (float)mouse->getPosX() / (float)context->getGraphics()->getViewport()->getWidth();
	float percentY = (float)mouse->getPosY() / (float)context->getGraphics()->getViewport()->getHeight();
	int imgX = percentX * context->getGraphics()->getFrameBufferWidth();
	int imgY = context->getGraphics()->getFrameBufferHeight() - (percentY * context->getGraphics()->getFrameBufferHeight());
	m_selectedPixel = context->getGraphics()->getRenderImagePixel(imgX, imgY, RenderOptions::SELECTION);
}

unsigned int Selection::getSelectionAsId()
{
	unsigned short r = m_selectedPixel[0] / 256;
	unsigned short g = m_selectedPixel[1] / 256;
	unsigned short b = m_selectedPixel[2] / 256;
	return r + (g * 255) + (b * 65025);
}

boost::shared_array<unsigned short> Selection::getSelectionAsColor()
{
	return m_selectedPixel;
}