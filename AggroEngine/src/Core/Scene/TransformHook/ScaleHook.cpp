#include "ScaleHook.hpp"

ScaleHook::ScaleHook()
	: LineHook()
{
}

void ScaleHook::updateTransform(shared_ptr<SceneNode> node, glm::vec3 &diff, const glm::vec3 &selectorDir)
{
	node->getTransformComponent()->scale(glm::vec3(1.0) + diff);
}

glm::mat4 ScaleHook::getRelativeTransform(shared_ptr<SceneNode> node)
{
	return node->getParentTransform() * node->getOrthogonalObjectTransform();
}