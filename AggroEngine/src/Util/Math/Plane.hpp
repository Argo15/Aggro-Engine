#pragma once

#include "Matrix.hpp"
#include "Line.hpp"
#include <memory>
#include <boost/optional.hpp>

class Plane
{
private:
	glm::vec3 m_center;
	glm::vec3 m_normal;

public:
	Plane(glm::vec3 center, glm::vec3 normal);

	const boost::optional<glm::vec3> intersect(const Line& line) const;
};