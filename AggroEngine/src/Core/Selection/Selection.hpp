#pragma once

#include "EngineContext.hpp"
#include "MouseState.hpp"

class EngineContext;

class Selection
{
private:
	unsigned int m_size;
	shared_ptr<ImageUC> m_selectedArea;
	float m_depthVal;

public:
	Selection(unsigned int m_size = 19);

	void updateSelection(shared_ptr<MouseState> mouse, shared_ptr<Graphics> graphics);

	unsigned int getSelectionAsId();
	unsigned int getSelectionAsId(unsigned int x, unsigned int y);
	shared_ptr<unsigned char> getSelectionAsColor();
	shared_ptr<unsigned char> getSelectionAsColor(unsigned int x, unsigned int y);
	unsigned int getSize();
	float getDepthVal(shared_ptr<MouseState> mouse, shared_ptr<Graphics> graphics);
};