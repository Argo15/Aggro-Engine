#include "TranslateHook.hpp"

TranslateHook::TranslateHook()
	: LineHook()
{
}

void TranslateHook::updateTransform(shared_ptr<TransformComponent> transform, glm::vec3 &diff)
{
	transform->translate(diff);
}