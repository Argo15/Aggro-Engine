#include "PerspectiveFrustrum.hpp"

PerspectiveFrustrum::PerspectiveFrustrum(
		glm::vec3 eyePos, glm::vec3 lookDir, glm::vec3 upDir, glm::vec3 rightDir,
		float fov, float aspectRatio, float zNear, float zFar, glm::mat4 &viewMatrix)
	: m_eyePos(eyePos)
	, m_lookDir(lookDir)
	, m_upDir(upDir)
	, m_rightDir(rightDir)
	, m_fov(fov)
	, m_aspectRatio(aspectRatio)
	, m_zNear(zNear)
	, m_zFar(zFar)
	, m_viewMatrix(viewMatrix)
{
	float nearHeight = 2 * tan(m_fov / 2) * m_zNear;
	float nearWidth = nearHeight * m_aspectRatio;

	float farHeight = 2 * tan(m_fov / 2) * m_zFar;
	float farWidth = farHeight * m_aspectRatio;

	glm::vec3 look = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 right = glm::vec3(1, 0, 0);

	glm::vec3 nearCenter = look * m_zNear;
	glm::vec3 farCenter = look * m_zFar;

	glm::vec3 *corners = new glm::vec3[8];
	corners[0] = nearCenter + (nearHeight / 2) * up + (nearWidth / 2) * right;
	corners[1] = nearCenter - (nearHeight / 2) * up + (nearWidth / 2) * right;
	corners[2] = nearCenter + (nearHeight / 2) * up - (nearWidth / 2) * right;
	corners[3] = nearCenter - (nearHeight / 2) * up - (nearWidth / 2) * right;
	corners[4] = farCenter + (farHeight / 2) * up + (farWidth / 2) * right;
	corners[5] = farCenter - (farHeight / 2) * up + (farWidth / 2) * right;
	corners[6] = farCenter + (farHeight / 2) * up - (farWidth / 2) * right;
	corners[7] = farCenter - (farHeight / 2) * up - (farWidth / 2) * right;
	m_corners = mem::shared_array(corners);

	Plane *planes = new Plane[6];
	planes[0] = Plane(nearCenter, -look); // near
	planes[1] = Plane(farCenter, look); // far
	planes[2] = Plane(corners[2], glm::cross( // left
		glm::normalize(corners[6] - corners[2]),
		glm::normalize(corners[3] - corners[2])));
	planes[3] = Plane(corners[0], glm::cross( // right
		glm::normalize(corners[1] - corners[0]),
		glm::normalize(corners[4] - corners[0])));
	planes[4] = Plane(corners[0], glm::cross( // up
		glm::normalize(corners[4] - corners[0]),
		glm::normalize(corners[2] - corners[0])));
	planes[5] = Plane(corners[1], glm::cross( // bottom
		glm::normalize(corners[7] - corners[1]),
		glm::normalize(corners[5] - corners[1])));
	m_planes = mem::shared_array(planes);
}

glm::vec3 PerspectiveFrustrum::getEyePos()
{
	return m_eyePos;
}

glm::vec3 PerspectiveFrustrum::getLookDir()
{
	return m_lookDir;
}

glm::vec3 PerspectiveFrustrum::getUpDir()
{
	return m_upDir;
}

glm::vec3 PerspectiveFrustrum::getRightDir()
{
	return m_rightDir;
}

float PerspectiveFrustrum::getFov()
{
	return m_fov;
}

float PerspectiveFrustrum::getAspectRatio()
{
	return m_aspectRatio;
}

float PerspectiveFrustrum::getZNear()
{
	return m_zNear;
}

float PerspectiveFrustrum::getZFar()
{
	return m_zFar;
}

shared_ptr<glm::vec3> PerspectiveFrustrum::getCorners()
{
	return m_corners;
}

bool PerspectiveFrustrum::isSame(shared_ptr<Frustrum> other)
{
	if (!other)
	{
		return false;
	}
	PerspectiveFrustrum *o = dynamic_cast<PerspectiveFrustrum*>(other.get());
	if (o == nullptr)
	{
		return false;
	}
	return m_fov == o->m_fov &&
		m_aspectRatio == o->m_aspectRatio &&
		m_zNear == o->m_zNear &&
		m_zFar == o->m_zFar;
}

FrustrumCulling PerspectiveFrustrum::getCulling(shared_ptr<glm::vec3> points, int size, glm::mat4 &modelMatrix)
{
	for (int j = 0; j < 6; j++)
	{
		bool allOutside = true;
		for (int i = 0; i < size; i++)
		{
			glm::vec3 point = glm::vec3(m_viewMatrix * modelMatrix * glm::vec4(points.get()[i], 1.0));
			allOutside = allOutside && m_planes.get()[j].distance(point) > 0;
		}
		if (allOutside)
		{
			return FrustrumCulling::OUTSIDE;
		}
	}
	return FrustrumCulling::INSIDE;
}
