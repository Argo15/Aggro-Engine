#pragma once

#include "CircleHook.hpp"

class RotateHook : public CircleHook
{
protected:
	void updateTransform(shared_ptr<SceneNode> node, const glm::vec3 &selectorDir, const float amount);

public:
	RotateHook();
};