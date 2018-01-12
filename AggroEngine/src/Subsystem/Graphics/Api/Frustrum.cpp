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