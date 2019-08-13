#include "RotateHook.hpp"

RotateHook::RotateHook()
	: CircleHook()
{
}

void RotateHook::updateTransform(shared_ptr<SceneNode> node, const glm::vec3 &selectorDir, const float amount)
{
	node->getTransformComponent()->rotate(amount, selectorDir, node->getMeshCenter());
}