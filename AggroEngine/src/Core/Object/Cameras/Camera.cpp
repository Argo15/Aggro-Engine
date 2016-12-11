#include "Camera.hpp"
#include <iostream>

static glm::vec4 baseEyePos(0.f,0.f,0.f,1.f);
static glm::vec4 baseLookDir(0.f,0.f,-1.f,0.f);
static glm::vec4 baseUpDir(0.f,1.f,0.f,0.f);
static glm::vec4 baseRightDir(1.f,0.f,0.f,0.f);

Camera::Camera()
	: Object()
	, m_v3EyePos(baseEyePos)
	, m_v3LookDir(baseLookDir)
	, m_v3UpDir(baseUpDir)
	, m_v3RightDir(baseRightDir)
{
	// Default position
	translate(glm::vec3(4.f, 4.f, 4.f));

	// Default look
	rotate(0.79f, m_v3UpDir);
	rotate(-0.61f, m_v3RightDir);
}

void Camera::updateViewMatrix()
{
	glm::mat4 transform = this->getTransform();
	m_v3EyePos = glm::vec3(transform * baseEyePos);
	m_v3LookDir = glm::vec3(transform * baseLookDir);
	m_v3UpDir = glm::vec3(transform * baseUpDir);
	m_v3RightDir = glm::vec3(transform * baseRightDir);
	m_m4ViewMatrix = glm::lookAt(m_v3EyePos, m_v3EyePos+m_v3LookDir, m_v3UpDir);
}

void Camera::translate(const glm::vec3 &translate)
{
	Object::translate(translate);
	updateViewMatrix();
}

void Camera::rotate(const glm::mat4 &rotate)
{
	Object::rotate(rotate);
	updateViewMatrix();
}

void Camera::rotate(float angle, const glm::vec3 &axis)
{
	Object::rotate(angle, axis);
	updateViewMatrix();
}

void Camera::scale(const glm::vec3 &scale)
{
	Object::scale(scale);
	updateViewMatrix();
}

void Camera::setProjection(glm::mat4 &m4Projection)
{
	m_m4ProjMatrix = m4Projection;
}

void Camera::setLookAt(glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up)
{
	// TODO, math this one out.
}

glm::mat4 &Camera::getViewMatrix()
{
	return m_m4ViewMatrix;
}

glm::mat4 &Camera::getProjMatrix()
{
	return m_m4ProjMatrix;
}

glm::vec3 &Camera::getEyePos()
{
	return m_v3EyePos;
}

glm::vec3 &Camera::getLookDir()
{
	return m_v3LookDir;
}

glm::vec3 &Camera::getUpDir()
{
	return m_v3UpDir;
}

glm::vec3 &Camera::getRightDir()
{
	return m_v3RightDir;
}