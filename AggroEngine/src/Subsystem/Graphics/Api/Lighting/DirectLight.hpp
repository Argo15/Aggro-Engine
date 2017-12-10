#pragma once

#include "Matrix.hpp"

class DirectLight
{
private:
	glm::vec3 m_direction;

public:
	DirectLight(glm::vec3 direction);

	glm::vec3 &getDirection();
};