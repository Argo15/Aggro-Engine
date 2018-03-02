#pragma once

#include "Matrix.hpp"

class MatrixDecompose
{
private:
	glm::vec3 m_scale;
	glm::quat m_rotate;
	glm::quat m_conjugatedRotate;
	glm::vec3 m_translate;
	glm::vec3 m_skew;
	glm::vec4 m_perspective;

public:
	MatrixDecompose(glm::mat4 transform);

	void setScale(glm::vec3 scale);
	void setRotate(glm::quat rotate);
	void setTranslate(glm::vec3 translate);
	void setSkew(glm::vec3 skew);
	void setPerspective(glm::vec4 perspective);

	glm::vec3 getScale();
	glm::quat getRotate();
	glm::quat getOriginalRotate();
	glm::vec3 getTranslate();
	glm::vec3 getSkew();
	glm::vec4 getPerspective();
	glm::mat4 getOrthogonalTransform();
};