#pragma once

#include "EngineContext.hpp"
#include "MouseState.hpp"

class EngineContext;

class Selection
{
private:
	boost::shared_array<unsigned short> m_selectedPixel;

public:
	Selection();

	void updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context);

	unsigned int getSelectionAsId();
	boost::shared_array<unsigned short> getSelectionAsColor();
};