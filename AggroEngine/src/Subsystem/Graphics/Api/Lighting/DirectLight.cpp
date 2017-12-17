#include "DirectLight.hpp"

DirectLight::DirectLight(glm::vec3 direction, glm::vec3 color, int ambientPct)
	: m_direction(direction)
	, m_color(color)
	, m_ambientPct(ambientPct)
{
}

glm::vec3 &DirectLight::getDirection()
{
	return m_direction;
}

glm::vec3 &DirectLight::getColor()
{
	return m_color;
}

int DirectLight::getAmbient()
{
	return m_ambientPct;
}