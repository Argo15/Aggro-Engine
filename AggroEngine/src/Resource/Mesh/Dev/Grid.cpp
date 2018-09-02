#include "Grid.hpp"
#include "MemoryUtil.hpp"

// Verts around the circumference
void Grid::_buildVerticies(int size)
{
	int radius = size / 2;
	for (int i = -radius; i <= radius; i++)
	{
		// Top
		m_verticies.push_back((float)i); // x
		m_verticies.push_back(0.f); // y
		m_verticies.push_back(radius); // z
		// Bottom
		m_verticies.push_back((float)i); // x
		m_verticies.push_back(0.f); // y
		m_verticies.push_back(-radius); // z
		// Left
		m_verticies.push_back(-radius); // x
		m_verticies.push_back(0.f); // y
		m_verticies.push_back((float)i); // z
		// Right
		m_verticies.push_back(radius); // x
		m_verticies.push_back(0.f); // y
		m_verticies.push_back((float)i); // z
	}
}

void Grid::_buildIndices(int size)
{
	std::vector<int> ret;
	for (int i = 0; i <= size; i++)
	{
		// Top
		m_indicies.push_back(4*i);
		// Bottom
		m_indicies.push_back(4*i+1);
		// Left
		m_indicies.push_back(4*i+2);
		// Right
		m_indicies.push_back(4*i+3);
	}
}

shared_ptr<float> Grid::_getVertices()
{
	float *arr = new float[m_verticies.size()];
	std::copy(m_verticies.begin(), m_verticies.end(), arr);
	return mem::shared_array(arr, m_verticies.size(), "Mesh");
}

// Inits to 0
shared_ptr<float> Grid::_getTexCoords()
{
	int size = m_verticies.size() * 2 / 3;
	float *arr = new float[size];
	for (int i = 0; i < size; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr, size, "Mesh");
}

// Inits to 0
shared_ptr<float> Grid::_getNormals()
{
	float *arr = new float[m_verticies.size()];
	for (int i = 0; i < m_verticies.size(); i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr, m_verticies.size(), "Mesh");
}

shared_ptr<int> Grid::_getIndices()
{
	int *arr = new int[m_indicies.size()];
	std::copy(m_indicies.begin(), m_indicies.end(), arr);
	return mem::shared_array(arr, m_verticies.size(), "Mesh");
}

Grid::Grid(int meshId, int size)
	: Mesh(meshId)
{
	_buildVerticies(size);
	_buildIndices(size);
	setSizeOfVerticies(sizeof(float) * m_verticies.size());
	setSizeOfIndicies(sizeof(int) * m_indicies.size());
	setVerticies(_getVertices());
	setTexCoords(_getTexCoords());
	setNormals(_getNormals());
	setIndicies(_getIndices());
	m_drawMode = DrawMode::LINES;
}