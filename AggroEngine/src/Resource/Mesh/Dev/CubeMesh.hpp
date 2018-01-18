#pragma once

#include "Mesh.hpp"

/**
 * Hard-coded cube mesh
 *
 * author: wcrane
 * since: 2014-01-25
**/
class CubeMesh : public Mesh
{
private:
	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	CubeMesh(int meshId);
};