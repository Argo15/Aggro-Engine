#pragma once

#include "Matrix.hpp"
#include "Plane.hpp"
#include "MemoryUtil.hpp"

enum FrustrumCulling
{
	INSIDE,
	OUTSIDE
};

/**
* Frustrum volume information
*
* author: wcrane
* since: 2018-03-22
**/
class Frustrum
{
public:
	virtual shared_ptr<glm::vec3> getCorners() = 0;
	virtual bool isSame(shared_ptr<Frustrum> other) = 0;
	virtual FrustrumCulling getCulling(shared_ptr<glm::vec3> points, int size, glm::mat4 &modelMatrix) = 0;
};