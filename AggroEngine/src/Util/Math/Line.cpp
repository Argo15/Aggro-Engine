#include "Line.hpp"

Line::Line(glm::vec3 pos, glm::vec3 dir)
	: m_pos(pos)
	, m_dir(dir)
{

}

// Point on the other line that is closest to this line
const glm::vec3 Line::nearestPoint(const Line& other) const
{
	glm::vec3 nearestPoint = other.m_pos;

	float a = glm::dot(m_dir, m_dir);
	float b = glm::dot(m_dir, other.m_dir);
	float e = glm::dot(other.m_dir, other.m_dir);

	float d = a*e - b*b;
	if (d != 0) // If the two lines are not parallel.
	{
		glm::vec3 r = m_pos - other.m_pos;

		float c = glm::dot(m_dir, r);
		float f = glm::dot(other.m_dir, r);

		float t = (a*f - b*c) / d;
		nearestPoint = t * other.m_dir + other.m_pos;
	}
	return nearestPoint;
}