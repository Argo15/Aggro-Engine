#include "ShadowMapFrustrum.hpp"

ShadowMapFrustrum::ShadowMapFrustrum(glm::mat4 &viewProjMat)
	: m_viewProjMat(viewProjMat)
{
}


shared_ptr<glm::vec3> ShadowMapFrustrum::getCorners()
{
	return shared_ptr<glm::vec3>();
}

bool ShadowMapFrustrum::isSame(shared_ptr<Frustrum> other)
{
	return false;
}

FrustrumCulling ShadowMapFrustrum::getCulling(shared_ptr<glm::vec3> points, int size, glm::mat4 &modelMatrix)
{
	glm::mat4 modelView = m_viewProjMat * modelMatrix;
	bool allOutsideTests[6];
	for (int i = 0; i < 6; i++)
	{
		allOutsideTests[i] = true;
	}
	for (int i = 0; i < size; i++)
	{
		glm::vec4 point = modelView * glm::vec4(points.get()[i], 1);
		allOutsideTests[0] = allOutsideTests[0] && point.x < -1;
		allOutsideTests[1] = allOutsideTests[1] && point.x > 1;
		allOutsideTests[2] = allOutsideTests[2] && point.y < -1;
		allOutsideTests[3] = allOutsideTests[3] && point.y > 1;
		allOutsideTests[4] = allOutsideTests[4] && point.z < -1;
		allOutsideTests[5] = allOutsideTests[5] && point.z > 1;
	}
	for (int i = 0; i < 6; i++)
	{
		if (allOutsideTests[i])
		{
			return FrustrumCulling::OUTSIDE;
		}
	}
	return FrustrumCulling::INSIDE;
}

