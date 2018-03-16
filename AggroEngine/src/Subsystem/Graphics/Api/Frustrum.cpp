#include "Frustrum.hpp"

Frustrum::Frustrum(glm::vec3 eyePos, glm::vec3 lookDir, glm::vec3 upDir, glm::vec3 rightDir,
				   float fov, float aspectRatio, float zNear, float zFar)
	: m_eyePos(eyePos)
	, m_lookDir(lookDir)
	, m_upDir(upDir)
	, m_rightDir(rightDir)
	, m_fov(fov)
	, m_aspectRatio(aspectRatio)
	, m_zNear(zNear)
	, m_zFar(zFar)
{
}

glm::vec3 Frustrum::getEyePos()
{
	return m_eyePos;
}

glm::vec3 Frustrum::getLookDir()
{
	return m_lookDir;
}

glm::vec3 Frustrum::getUpDir()
{
	return m_upDir;
}

glm::vec3 Frustrum::getRightDir()
{
	return m_rightDir;
}

float Frustrum::getFov()
{
	return m_fov;
}

float Frustrum::getAspectRatio()
{
	return m_aspectRatio;
}

float Frustrum::getZNear()
{
	return m_zNear;
}

float Frustrum::getZFar()
{
	return m_zFar;
}

shared_ptr<glm::vec3> Frustrum::getCorners()
{
	if (m_corners)
	{
		return m_corners;
	}

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
	return m_corners;
}

bool Frustrum::isSame(shared_ptr<Frustrum> other)
{
	if (!other)
	{
		return false;
	}
	return m_fov == other->m_fov &&
		m_aspectRatio == other->m_aspectRatio &&
		m_zNear == other->m_zNear &&
		m_zFar == other->m_zFar;
}