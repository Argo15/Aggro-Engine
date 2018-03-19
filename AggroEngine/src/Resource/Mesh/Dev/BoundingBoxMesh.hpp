#pragma once

#include <vector>
#include "Mesh.hpp"
#include "Matrix.hpp"

/**
* Builds a line mesh that shows a box given 8 corners.
*
* author: wcrane
* since: 2018-03-18
**/
class BoundingBoxMesh : public Mesh
{
private:
	shared_ptr<glm::vec3> m_corners;

	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	BoundingBoxMesh(int meshId, shared_ptr<glm::vec3> corners);
};