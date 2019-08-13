#pragma once

#include "TransformHook.hpp"
#include "Line.hpp"

class LineHook : public TransformHook
{
protected:
	virtual void updateTransform(shared_ptr<SceneNode> selectedNode, const Line &lookLine, const glm::vec3 &axis);
	virtual void updateTransform(shared_ptr<SceneNode> node, glm::vec3 &diff, const glm::vec3 &selectorDir) = 0;
	virtual glm::mat4 getRelativeTransform(shared_ptr<SceneNode> node);
	virtual shared_ptr<Mesh> getMesh(glm::vec3 axis);

public:
	LineHook();
};