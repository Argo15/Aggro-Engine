#pragma once

#include "Matrix.hpp"

class Line
{
private:
	glm::vec3 m_pos;
	glm::vec3 m_dir;

public:
	Line(glm::vec3 pos, glm::vec3 dir);

	const glm::vec3 nearestPoint(const Line& other) const;

	glm::vec3 getOrig() const;
	glm::vec3 getDir() const;
};