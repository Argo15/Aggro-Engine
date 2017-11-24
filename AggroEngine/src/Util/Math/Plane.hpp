#pragma once

#include "Matrix.hpp"
#include "Line.hpp"
#include <memory>

class Plane
{
private:
	glm::vec3 m_center;
	glm::vec3 m_normal;

public:
	Plane(glm::vec3 center, glm::vec3 normal);

	const std::shared_ptr<glm::vec3> intersect(const Line& line) const;
};