#pragma once

#include "Camera.hpp"
#include "KeyboardState.hpp"
#include "MouseState.hpp"
#include "SceneNode.hpp"

class CameraController : public boost::basic_lockable_adapter<recursive_mutex>
{
public:
	CameraController();
	
	virtual void handleKeyboardInput(shared_ptr<SceneNode> cameraNode, KeyboardState &keyboard, float nSpeed) = 0;
	virtual void handleMouseInput(shared_ptr<SceneNode> cameraNode, MouseState &mouse, float nSpeed) = 0;
};