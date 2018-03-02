#pragma once

class GraphicsInitOptions
{
private:
	int m_bufferWidth;
	int m_bufferHeight;
	int m_shadowSize;

public:
	GraphicsInitOptions();

	GraphicsInitOptions *setBufferDimensions(int width, int height);
	GraphicsInitOptions *setShadowSize(int dim);

	int getBufferWidth();
	int getBufferHeight();
	int getShadowSize();
};