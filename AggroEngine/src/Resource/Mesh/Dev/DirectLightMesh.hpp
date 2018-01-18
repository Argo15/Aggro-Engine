#pragma once

#include <vector>
#include "Mesh.hpp"
#include "Matrix.hpp"

/**
* author: wcrane
* since: 2017-12-08
**/
class DirectLightMesh : public Mesh
{
private:
	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	DirectLightMesh(int meshId);
};