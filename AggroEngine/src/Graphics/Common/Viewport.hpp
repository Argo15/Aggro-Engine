#pragma once

class Viewport
{
private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;

public:
	Viewport();

	int getX();
	int getY();
	int getWidth();
	int getHeight();

	void setDimensions(int x, int y, int width, int height);
};