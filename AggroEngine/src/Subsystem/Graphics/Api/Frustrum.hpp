#pragma once

#include "Matrix.hpp"

/**
* Frustrum volume information
*
* author: wcrane
* since: 2017-12-28
**/
class Frustrum
{
private:
	glm::vec3 m_eyePos;
	glm::vec3 m_lookDir;
	glm::vec3 m_upDir;
	glm::vec3 m_rightDir;
	float m_fov;
	float m_aspectRatio;
	float m_zNear; 
	float m_zFar;

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
};