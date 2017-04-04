#pragma once

#include "CameraController.hpp"

class FreeRoamCameraController : public CameraController
{
public:
	FreeRoamCameraController();
	
	void handleKeyboardInput(shared_ptr<Camera> camera, KeyboardState &keyboard, float nSpeed);
	void handleMouseInput(shared_ptr<Camera> camera, MouseState &mouse, float nSpeed);
};