#pragma once
#pragma once

#include "Frustrum.hpp"

/**
* Orthogonal Frustrum for shadow map culling
*
* author: wcrane
* since: 2018-03-23
**/
class ShadowMapFrustrum : public Frustrum
{
private:
	glm::mat4 m_viewProjMat;

public:
	ShadowMapFrustrum(glm::mat4 &viewProjMat);

	virtual shared_ptr<glm::vec3> getCorners();
	virtual bool isSame(shared_ptr<Frustrum> other);
	virtual FrustrumCulling getCulling(shared_ptr<glm::vec3> points, int size, glm::mat4 &modelMatrix);
};