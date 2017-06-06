#include "ScaleHook.hpp"

ScaleHook::ScaleHook()
	: LineHook()
{
}

void ScaleHook::updateTransform(shared_ptr<TransformComponent> transform, glm::vec3 &diff)
{
	transform->scale(glm::vec3(1.0) + diff);
}

glm::mat4 ScaleHook::getRelativeTransform(shared_ptr<SceneNode> node)
{
	return node->getParentTransform() * node->getOrthogonalObjectTransform();
}