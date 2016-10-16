#pragma once

#include "Camera.hpp"
#include "KeyboardState.hpp"
#include "MouseState.hpp"

class CameraController
{
public:
	CameraController();
	
	virtual void handleKeyboardInput(Camera &camera, KeyboardState &keyboard, float nSpeed) = 0;
	virtual void handleMouseInput(Camera &camera, MouseState &mouse, float nSpeed) = 0;
};