#pragma once

#include "CameraController.hpp"

class FreeRoamCameraController : public CameraController
{
public:
	FreeRoamCameraController();
	
	void handleKeyboardInput(shared_ptr<SceneNode> cameraNode, KeyboardState &keyboard, float nSpeed);
	void handleMouseInput(shared_ptr<SceneNode> cameraNode, MouseState &mouse, float nSpeed);
};