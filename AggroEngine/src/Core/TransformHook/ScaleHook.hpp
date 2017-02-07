#pragma once

#include "TranslateHook.hpp"

class ScaleHook : public LineHook
{
protected:
	void updateTransform(shared_ptr<TransformComponent> transform, glm::vec3 &diff);

public:
	ScaleHook();
};