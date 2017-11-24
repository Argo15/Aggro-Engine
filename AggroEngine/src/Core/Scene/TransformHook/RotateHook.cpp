#include "RotateHook.hpp"

RotateHook::RotateHook()
	: CircleHook()
{
}

void RotateHook::updateTransform(shared_ptr<TransformComponent> transform, const glm::vec3 &selectorDir, const float amount)
{
	transform->rotate(amount, selectorDir);
}