#pragma once

#include <vector>
#include "Mesh.hpp"
#include "Matrix.hpp"

/**
* Builds a mesh for a single line.
*
* author: wcrane
* since: 2016-11-12
**/
class LineMesh : public Mesh
{
private:
	glm::vec3 m_p1;
	glm::vec3 m_p2;
	std::vector<float> m_verticies;
	std::vector<int> m_indicies;

	void _buildVerticies();
	void _buildIndices();

	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	LineMesh(glm::vec3 p1, glm::vec3 p2);
};