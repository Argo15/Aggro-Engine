#pragma once

#include "LineHook.hpp"

class ScaleHook : public LineHook
{
protected:
	void updateTransform(shared_ptr<TransformComponent> transform, glm::vec3 &diff, const glm::vec3 &selectorDir);
	glm::mat4 getRelativeTransform(shared_ptr<SceneNode> node);

public:
	ScaleHook();
};