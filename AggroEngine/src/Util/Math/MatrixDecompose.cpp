#include "MatrixDecompose.hpp"

MatrixDecompose::MatrixDecompose(glm::mat4 transform)
{
	glm::decompose(transform, m_scale, m_rotate, m_translate, m_skew, m_perspective);
	m_rotate = glm::conjugate(m_rotate);
}

glm::vec3 MatrixDecompose::getScale()
{
	return m_scale;
}

glm::quat MatrixDecompose::getRotate()
{
	return m_rotate;
}

glm::vec3 MatrixDecompose::getTranslate()
{
	return m_translate;
}

glm::vec3 MatrixDecompose::getSkew()
{
	return m_skew;
}

glm::vec4 MatrixDecompose::getPerspective()
{
	return m_perspective;
}

// Just rotate and translate
glm::mat4 MatrixDecompose::getOrthogonalTransform()
{
	return glm::translate(glm::mat4(1.0), m_translate) * mat4_cast(m_rotate);
}