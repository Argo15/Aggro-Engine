#pragma once

#include "Camera.hpp"
#include "../../../Inputs/KeyboardState.hpp"
#include "../../../Inputs/MouseState.hpp"

class CameraController
{
public:
	CameraController();
	
	virtual void handleKeyboardInput(Camera &camera, KeyboardState &keyboard, float nSpeed) = 0;
	virtual void handleMouseInput(Camera &camera, MouseState &mouse, float nSpeed) = 0;
};