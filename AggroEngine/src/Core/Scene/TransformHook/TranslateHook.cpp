#include "TranslateHook.hpp"

TranslateHook::TranslateHook()
	: LineHook()
{
}

void TranslateHook::updateTransform(shared_ptr<SceneNode> node, glm::vec3 &diff, const glm::vec3 &selectorDir)
{
	node->getTransformComponent()->translate(diff);
}