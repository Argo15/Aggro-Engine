#pragma once

#include "Matrix.hpp"

class DirectLight
{
private:
	glm::vec3 m_direction;
	glm::vec3 m_color;
	int m_ambientPct;

public:
	DirectLight(glm::vec3 direction, glm::vec3 color, int ambientPct);

	glm::vec3 &getDirection();
	glm::vec3 &getColor();
	int getAmbient();
};