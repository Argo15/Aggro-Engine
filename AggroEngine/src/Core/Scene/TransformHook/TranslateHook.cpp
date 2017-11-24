#include "TranslateHook.hpp"

TranslateHook::TranslateHook()
	: LineHook()
{
}

void TranslateHook::updateTransform(shared_ptr<TransformComponent> transform, glm::vec3 &diff, const glm::vec3 &selectorDir)
{
	transform->translate(diff);
}