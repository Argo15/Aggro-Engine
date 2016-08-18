#pragma once

#include <vector>
#include "Mesh.hpp"

/**
* Hard-coded line grid
*
* author: wcrane
* since: 2014-01-25
**/
class Grid : public Mesh
{
private:
	std::vector<float> m_verticies;
	std::vector<int> m_indicies;

	void _buildVerticies(int size);
	void _buildIndices(int size);

	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	Grid(int size);
};