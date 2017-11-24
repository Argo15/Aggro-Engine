#pragma once

#include "TransformHook.hpp"
#include "Line.hpp"

class CircleHook : public TransformHook
{
protected:
	virtual void updateTransform(shared_ptr<SceneNode> node, const Line &lookLine, const glm::vec3 &axis);
	virtual void updateTransform(shared_ptr<TransformComponent> transform, const glm::vec3 &selectorDir, const float amount) = 0;
	virtual shared_ptr<Mesh> getMesh(glm::vec3 axis);

public:
	CircleHook();
};