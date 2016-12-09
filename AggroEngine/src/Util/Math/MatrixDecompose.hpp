#pragma once

#include "Matrix.hpp"

class MatrixDecompose
{
private:
	glm::vec3 m_scale;
	glm::quat m_rotate;
	glm::vec3 m_translate;
	glm::vec3 m_skew;
	glm::vec4 m_perspective;

public:
	MatrixDecompose(glm::mat4 transform);

	glm::vec3 getScale();
	glm::quat getRotate();
	glm::vec3 getTranslate();
	glm::vec3 getSkew();
	glm::vec4 getPerspective();
	glm::mat4 getOrthogonalTransform();
};