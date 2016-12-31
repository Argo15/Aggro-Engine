#pragma once

#include "EngineContext.hpp"
#include "MouseState.hpp"

class EngineContext;

class Selection
{
private:
	unsigned int m_size;
	shared_ptr<Image> m_selectedArea;

public:
	Selection(unsigned int m_size = 19);

	void updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context);

	unsigned int getSelectionAsId();
	unsigned int getSelectionAsId(unsigned int x, unsigned int y);
	boost::shared_array<unsigned short> getSelectionAsColor();
	boost::shared_array<unsigned short> getSelectionAsColor(unsigned int x, unsigned int y);
	unsigned int getSize();
};