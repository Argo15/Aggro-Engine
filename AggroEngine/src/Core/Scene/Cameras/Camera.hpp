#pragma once

#include "TransformComponent.hpp"
#include "Matrix.hpp"
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
	glm::vec3 m_v3EyePos;
	glm::vec3 m_v3LookDir;
	glm::vec3 m_v3UpDir;
	glm::vec3 m_v3RightDir;
	glm::mat4 m_m4ViewMatrix;
	glm::mat4 m_m4ProjMatrix;

	shared_ptr<TransformComponent> m_transformComponent;

	void updateViewMatrix();

public:
	Camera();

	void translate(const glm::vec3 &translate);
	void rotate(float angle, const glm::vec3 &axis);
	void scale(const glm::vec3 &scale);
	
	void setProjection(glm::mat4 &m4Projection);

	void setLookAt(glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up);

	glm::mat4 &getViewMatrix();
	glm::mat4 &getProjMatrix();
	glm::vec3 &getEyePos();
	glm::vec3 &getLookDir();
	glm::vec3 &getUpDir();
	glm::vec3 &getRightDir();
};