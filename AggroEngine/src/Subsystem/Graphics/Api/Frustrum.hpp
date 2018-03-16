#pragma once

#include "Matrix.hpp"
#include "MemoryUtil.hpp"

/**
* Frustrum volume information
*
* author: wcrane
* since: 2017-12-28
**/
class Frustrum
{
private:
	const glm::vec3 m_eyePos;
	const glm::vec3 m_lookDir;
	const glm::vec3 m_upDir;
	const glm::vec3 m_rightDir;
	const float m_fov;
	const float m_aspectRatio;
	const float m_zNear;
	const float m_zFar;
	shared_ptr<glm::vec3> m_corners;

public:
	Frustrum(glm::vec3 eyePos, glm::vec3 lookDir, glm::vec3 upDir, glm::vec3 rightDir, 
		float fov, float aspectRatio, float zNear, float zFar);

	glm::vec3 getEyePos();
	glm::vec3 getLookDir();
	glm::vec3 getUpDir();
	glm::vec3 getRightDir();
	float getFov();
	float getAspectRatio();
	float getZNear();
	float getZFar();

	shared_ptr<glm::vec3> getCorners();

	bool isSame(shared_ptr<Frustrum> other);
};