#pragma once

#include "Matrix.hpp"
#include "Frustrum.hpp"
#include <memory>
using namespace std;

/**
 * An object to help camera view transformations
 *
 * author: wcrane
 * since: 2013-12-14
**/
class Camera
{
private:
	glm::mat4 m_m4ViewMatrix;
	glm::mat4 m_m4ProjMatrix;
	glm::vec4 m_v4Viewport;
	glm::vec3 m_eyePos;
	glm::vec3 m_lookDir;
	glm::vec3 m_upDir;
	glm::vec3 m_rightDir;
	shared_ptr<Frustrum> m_frustrum;

public:
	Camera();
	
	Camera *setViewMatrix(glm::mat4 &viewMatrix);
	Camera *setProjMatrix(glm::mat4 &projMatrix);
	Camera *setViewport(glm::vec4 &viewport);
	Camera *setEyePos(glm::vec3 &eyePos);
	Camera *setLookDir(glm::vec3 &lookDir);
	Camera *setUpDir(glm::vec3 &upDir);
	Camera *setRightDir(glm::vec3 &rightDir);
	Camera *setFrustrum(shared_ptr<Frustrum> frustrum);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix();
	glm::vec4 getViewport();
	glm::vec3 getEyePos();
	glm::vec3 getLookDir();
	glm::vec3 getUpDir();
	glm::vec3 getRightDir();
	shared_ptr<Frustrum> getFrustrum();
};