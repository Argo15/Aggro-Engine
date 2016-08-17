#pragma once

#include "../../Util/Math/Matrix.hpp"

/**
 * Options passed into start of the rendering execution.
 *
 * author: wcrane
 * since: 2013-12-14
**/
class RenderOptions
{
private:
	glm::mat4 m_m4ProjectionMatrix; // Ortho or perspective
	glm::mat4 m_m4ViewMatrix;  // Camera position/orientation

public:
	RenderOptions();

	RenderOptions &setProjectionMatrix(glm::mat4 &m4Projection);
	RenderOptions &setViewMatrix(glm::mat4 &m4View);

	glm::mat4 &getProjectionMatrix();
	glm::mat4 &getViewMatrix();
};