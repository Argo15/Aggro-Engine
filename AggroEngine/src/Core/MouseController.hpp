#pragma once

#include "MouseState.hpp"
#include "EngineContext.hpp"
#include <memory>
using namespace std;

class MouseController
{
public:
	MouseController();

	void handleMouseInput(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context);
};