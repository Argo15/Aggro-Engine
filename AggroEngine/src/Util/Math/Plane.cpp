#include "Plane.hpp"

Plane::Plane(glm::vec3 center, glm::vec3 normal)
	: m_center(center)
	, m_normal(glm::normalize(normal))
{

}

// Point on the other line that is closest to this line
const std::shared_ptr<glm::vec3> Plane::intersect(const Line& line) const
{
	glm::vec3 lineDir = glm::normalize(line.getDir());
	
	float denom = glm::dot(m_normal, lineDir);
	if (abs(denom) > 0)
	{
		float t = dot(m_center - line.getOrig(), m_normal) / denom;
		if (t >= 0) {
			return std::shared_ptr<glm::vec3>(new glm::vec3(line.getOrig() + t * lineDir));
		}
	}
	return std::shared_ptr<glm::vec3>();
}