#pragma once

#include "LineHook.hpp"

class TranslateHook : public LineHook
{
protected:
	void updateTransform(shared_ptr<TransformComponent> transform, glm::vec3 &diff, const glm::vec3 &selectorDir);

public:
	TranslateHook();
};