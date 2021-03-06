#include "Plane.hpp"

Plane::Plane()
{
}

Plane::Plane(glm::vec3 center, glm::vec3 normal)
	: m_center(center)
	, m_normal(glm::normalize(normal))
{

}

// Point on the other line that is closest to this line
const boost::optional<glm::vec3> Plane::intersect(const Line& line) const
{
	glm::vec3 lineDir = glm::normalize(line.getDir());
	
	float denom = glm::dot(m_normal, lineDir);
	if (abs(denom) > 0)
	{
		float t = dot(m_center - line.getOrig(), m_normal) / denom;
		if (t >= 0) {
			return boost::optional<glm::vec3>(line.getOrig() + t * lineDir);
		}
	}
	return boost::optional<glm::vec3>();
}

const float Plane::distance(const glm::vec3& point) const
{
	glm::vec3 pointVector = point - m_center;
	return glm::dot(pointVector, m_normal);
}