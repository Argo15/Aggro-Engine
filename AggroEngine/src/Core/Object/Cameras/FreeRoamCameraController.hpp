#pragma once

#include "CameraController.hpp"

class FreeRoamCameraController : public CameraController
{
public:
	FreeRoamCameraController();
	
	void handleKeyboardInput(Camera &camera, KeyboardState &keyboard, float nSpeed);
	void handleMouseInput(Camera &camera, MouseState &mouse, float nSpeed);
};