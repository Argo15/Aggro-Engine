#pragma once
#pragma once

#include "Frustrum.hpp"

/**
* Frustrum volume information
*
* author: wcrane
* since: 2017-12-28
**/
class PerspectiveFrustrum : public Frustrum
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
	glm::mat4 m_viewMatrix;
	shared_ptr<glm::vec3> m_corners;
	shared_ptr<Plane> m_planes;

public:
	PerspectiveFrustrum(glm::vec3 eyePos, glm::vec3 lookDir, glm::vec3 upDir, glm::vec3 rightDir,
		float fov, float aspectRatio, float zNear, float zFar, glm::mat4 &viewMatrix);

	glm::vec3 getEyePos();
	glm::vec3 getLookDir();
	glm::vec3 getUpDir();
	glm::vec3 getRightDir();
	float getFov();
	float getAspectRatio();
	float getZNear();
	float getZFar();

	virtual shared_ptr<glm::vec3> getCorners();
	virtual bool isSame(shared_ptr<Frustrum> other);
	virtual FrustrumCulling getCulling(shared_ptr<glm::vec3> points, int size, glm::mat4 &modelMatrix);
};