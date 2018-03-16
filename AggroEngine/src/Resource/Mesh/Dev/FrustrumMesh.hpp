#pragma once

#include <vector>
#include "Mesh.hpp"
#include "Matrix.hpp"
#include "Frustrum.hpp"

/**
* author: wcrane
* since: 2018-03-13
**/
class FrustrumMesh : public Mesh
{
private:
	shared_ptr<Frustrum> m_frustrum;

	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	FrustrumMesh(int meshId, shared_ptr<Frustrum> frustrum);
};