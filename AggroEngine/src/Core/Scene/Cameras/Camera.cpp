#include "Camera.hpp"
#include "MatrixDecompose.hpp"

static glm::vec4 baseEyePos(0.f,0.f,0.f,1.f);
static glm::vec4 baseLookDir(0.f,0.f,-1.f,0.f);
static glm::vec4 baseUpDir(0.f,1.f,0.f,0.f);
static glm::vec4 baseRightDir(1.f,0.f,0.f,0.f);

Camera::Camera()
{
}

Camera *Camera::setViewMatrix(glm::mat4 &viewMatrix)
{
	m_m4ViewMatrix = viewMatrix;
	return this;
}

Camera *Camera::setProjMatrix(glm::mat4 &projMatrix)
{
	m_m4ProjMatrix = projMatrix;
	return this;
}

Camera *Camera::setViewport(glm::vec4 &viewport)
{
	m_v4Viewport = viewport;
	return this;
}

Camera *Camera::setFrustrum(shared_ptr<Frustrum> frustrum)
{
	m_frustrum = frustrum;
	return this;
}

Camera *Camera::setEyePos(glm::vec3 &eyePos)
{
	m_eyePos = eyePos;
	return this;
}

Camera *Camera::setLookDir(glm::vec3 &lookDir)
{
	m_lookDir = lookDir;
	return this;
}

Camera *Camera::setUpDir(glm::vec3 &upDir)
{
	m_upDir = upDir;
	return this;
}

Camera *Camera::setRightDir(glm::vec3 &rightDir)
{
	m_rightDir = rightDir;
	return this;
}

glm::mat4 Camera::getViewMatrix()
{
	return m_m4ViewMatrix;
}

glm::mat4 Camera::getProjMatrix()
{
	return m_m4ProjMatrix;
}

glm::vec4 Camera::getViewport()
{
	return m_v4Viewport;
}

glm::vec3 Camera::getEyePos()
{
	return m_eyePos;
}

glm::vec3 Camera::getLookDir()
{
	return m_lookDir;
}

glm::vec3 Camera::getUpDir()
{
	return m_upDir;
}

glm::vec3 Camera::getRightDir()
{
	return m_rightDir;
}

shared_ptr<Frustrum> Camera::getFrustrum()
{
	return m_frustrum;
}
