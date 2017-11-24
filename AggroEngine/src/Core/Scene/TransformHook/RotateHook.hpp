#pragma once

#include "CircleHook.hpp"

class RotateHook : public CircleHook
{
protected:
	void updateTransform(shared_ptr<TransformComponent> transform, const glm::vec3 &selectorDir, const float amount);

public:
	RotateHook();
};