#include "TranslateHook.hpp"
#include "LineMesh.hpp"
#include "MatrixDecompose.hpp"

LineHook::LineHook()
	: TransformHook()
{
}

void LineHook::updateTransform(shared_ptr<SceneNode> selectedNode, const Line &lookLine, const glm::vec3 &axis)
{
	// Get selected line
	MatrixDecompose worldMat(selectedNode->getWorldTransform());
	const glm::vec3 selectionDir = worldMat.getRotate() * axis;
	const Line selectionLine(worldMat.getTranslate(), selectionDir);

	// Calculate change in mouse position across selected line
	const glm::vec3 newPos = lookLine.nearestPoint(selectionLine);
	const glm::mat4 relativeSpaceTransform = glm::inverse(getRelativeTransform(selectedNode));
	const glm::vec3 newPosInRelativeSpace = relativeSpaceTransform * glm::vec4(newPos, 1.0);
	if (m_lastPos)
	{
		glm::vec3 diff = newPosInRelativeSpace - *m_lastPos;
		if (glm::length(diff) > 0)
		{
			updateTransform(selectedNode->getTransformComponent(), diff, selectionDir);
		}
	}
	m_lastPos = shared_ptr<glm::vec3>(new glm::vec3(newPosInRelativeSpace));
}

glm::mat4 LineHook::getRelativeTransform(shared_ptr<SceneNode> node)
{
	return node->getParentTransform();
}

shared_ptr<Mesh> LineHook::getMesh(glm::vec3 axis)
{
	return shared_ptr<Mesh>(new LineMesh(-1, glm::vec3(0), 2.0f * axis));
}