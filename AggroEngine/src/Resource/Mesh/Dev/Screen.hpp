#pragma once

#include "Mesh.hpp"

class Screen : public Mesh
{
private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;

	shared_ptr<float> _getVertices();
	shared_ptr<float> _getTexCoords();
	shared_ptr<float> _getNormals();
	shared_ptr<int> _getIndices();

public:
	Screen(int x, int y, int width, int height);
};