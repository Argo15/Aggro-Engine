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
public:
	enum RenderMode
	{
		SHADED, ALBEDO, NORMAL, SELECTION
	};

private:
	glm::mat4 m_m4ProjectionMatrix; // Ortho or perspective
	glm::mat4 m_m4ViewMatrix;  // Camera position/orientation

	int resWidth;
	int resHeight;

	RenderMode m_renderMode;

public:
	RenderOptions();

	RenderOptions &setProjectionMatrix(glm::mat4 &m4Projection);
	RenderOptions &setViewMatrix(glm::mat4 &m4View);

	glm::mat4 &getProjectionMatrix();
	glm::mat4 &getViewMatrix();

	void setRenderMode(RenderMode mode);
	RenderMode getRenderMode();

	int getResolutionWidth();
	int getResolutionHeight();
};