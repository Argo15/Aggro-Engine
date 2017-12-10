#include "DirectLight.hpp"

DirectLight::DirectLight(glm::vec3 direction)
	: m_direction(direction)
{
}

glm::vec3 &DirectLight::getDirection()
{
	return m_direction;
}