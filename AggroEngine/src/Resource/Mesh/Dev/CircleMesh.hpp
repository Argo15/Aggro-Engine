#pragma once

#include <vector>
#include "Mesh.hpp"
#include "Matrix.hpp"

/**
* Builds a mesh for a circle.
*
* author: wcrane
* since: 2017-11-22
**/
class CircleMesh : public Mesh
{
private:
	glm::vec3 m_center;
	glm::vec3 m_axis;
	float m_size;
	int m_sides;

	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	CircleMesh(int meshId, glm::vec3 center, glm::vec3 axis, float size, int sides);
};