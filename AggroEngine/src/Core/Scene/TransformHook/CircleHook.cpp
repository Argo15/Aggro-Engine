#include "CircleHook.hpp"
#include "CircleMesh.hpp"
#include "MatrixDecompose.hpp"
#include "StringUtil.hpp"
#include "Plane.hpp"

CircleHook::CircleHook()
	: TransformHook()
{
}

void CircleHook::updateTransform(shared_ptr<SceneNode> node, const Line &lookLine, const glm::vec3 &axis)
{
	// Get selection plane
	MatrixDecompose worldMat(node->getWorldTransform());
	const glm::vec3 selectionDir = worldMat.getRotate() * axis;
	const Plane selectionPlane(worldMat.getTranslate(), selectionDir);

	// Calculate change in mouse position along circle's plane
	const boost::optional<glm::vec3> newPos = selectionPlane.intersect(lookLine);
	if (!newPos)
	{
		return;
	}
	const glm::mat4 relativeSpaceTransform = glm::inverse(node->getParentTransform());
	const glm::vec3 newPosInRelativeSpace = (glm::vec3) (relativeSpaceTransform * glm::vec4(*newPos, 1.0)) - node->getTransformComponent()->getTranslate();

	if (m_lastPos)
	{
		// position is centered at 0,0,0. Calculate axis ang change in angle 
		glm::vec3 lastVec = glm::normalize(*m_lastPos);
		glm::vec3 newVec = glm::normalize(newPosInRelativeSpace);
		float angle = acos(glm::dot(lastVec, newVec));

		if (angle > 0.01 && lastVec != newVec)
		{
			glm::vec3 rotAxis = glm::normalize(glm::cross(lastVec, newVec));
			updateTransform(node->getTransformComponent(), rotAxis, angle);
		}
	}
	m_lastPos = shared_ptr<glm::vec3>(new glm::vec3(newPosInRelativeSpace));
}

shared_ptr<Mesh> CircleHook::getMesh(glm::vec3 axis)
{
	return shared_ptr<Mesh>(new CircleMesh(-1, glm::vec3(0), axis, 1.5f, 50));
}