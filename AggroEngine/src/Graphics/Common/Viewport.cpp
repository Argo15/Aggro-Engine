#include "Viewport.hpp"

Viewport::Viewport()
{

}

int Viewport::getX()
{
	return m_x;
}

int Viewport::getY()
{
	return m_y;
}

int Viewport::getWidth()
{
	return m_width;
}

int Viewport::getHeight()
{
	return m_height;
}

void Viewport::setDimensions(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}
