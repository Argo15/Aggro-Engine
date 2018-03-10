#pragma once

#include "Matrix.hpp"
#include "DirectLight.hpp"
#include "Frustrum.hpp"
#include "Camera.hpp"
#include <memory>
using namespace std;

/**
 * Options passed into start of the rendering execution.
 *
 * author: wcrane
 * since: 2013-12-14
**/
class RenderOptions
{
public:
	enum RenderTarget
	{
		SHADED, ALBEDO, NORMAL, LIGHTING, GLOW, SELECTION, DIRECT_LIGHT, DEPTH
	};

private:
	glm::mat4 m_m4ProjectionMatrix; // Ortho or perspective
	glm::mat4 m_m4ViewMatrix;  // Camera position/orientation
	glm::vec4 m_v4Viewport; // Dimensions of window

	shared_ptr<Frustrum> m_frustrum;

	RenderTarget m_renderTarget;

	shared_ptr<DirectLight> m_directLight;

public:
	RenderOptions();

	void clear();

	RenderOptions &setProjectionMatrix(glm::mat4 &m4Projection);
	RenderOptions &setViewMatrix(glm::mat4 &m4View);
	RenderOptions &setViewport(glm::vec4 &m4Viewport);
	RenderOptions &setFrustrum(shared_ptr<Frustrum> frustrum);
	RenderOptions &setCamera(shared_ptr<Camera> camera);

	const glm::mat4 &getProjectionMatrix();
	const glm::mat4 &getViewMatrix();
	const glm::vec4 &getViewport();
	const shared_ptr<Frustrum> &getFrustrum();

	void setRenderTarget(RenderTarget target);
	RenderTarget getRenderTarget();

	bool hasDirectLight();
	void addDirectLight(shared_ptr<DirectLight> directLight);
	shared_ptr<DirectLight> getDirectLight();
};