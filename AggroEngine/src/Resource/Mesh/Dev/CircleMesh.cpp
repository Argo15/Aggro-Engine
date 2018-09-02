#include "CircleMesh.hpp"
#include "MemoryUtil.hpp"
#include <math.h> 

static const double pi(3.14159265);
static const glm::vec3 up(0, 1.f, 0);
static const glm::vec3 side(1.f, 0, 0);

CircleMesh::CircleMesh(int meshId, glm::vec3 center, glm::vec3 axis, float size, int sides)
	: Mesh(meshId)
	, m_center(center)
	, m_axis(glm::normalize(axis))
	, m_size(size)
	, m_sides(sides)
{
	setSizeOfVerticies(sizeof(float) * 6 * m_sides);
	setSizeOfIndicies(sizeof(int) * 2 * m_sides);
	setVerticies(_getVertices());
	setTexCoords(_getTexCoords());
	setNormals(_getNormals());
	setIndicies(_getIndices());
	m_drawMode = DrawMode::LINES;
}

shared_ptr<float> CircleMesh::_getVertices()
{
	glm::vec3 right = up != m_axis ? glm::cross(up, m_axis) : glm::cross(side, m_axis);
	glm::vec3 forward = glm::cross(m_axis, right);
	float *arr = new float[6 * m_sides];
	for (int i = 0; i < m_sides; i++)
	{
		// First point
		float pct = (double)i / m_sides;
		float xAxis = m_size * cos(pct * 2 * pi);
		float yAxis = m_size * sin(pct * 2 * pi);
		arr[6 * i] = m_center[0] + xAxis * right[0] + yAxis * forward[0];
		arr[6 * i + 1] = m_center[1] + xAxis * right[1] + yAxis * forward[1];
		arr[6 * i + 2] = m_center[2] + xAxis * right[2] + yAxis * forward[2];

		// Second point
		pct = (double)(i + 1) / m_sides;
		xAxis = m_size * cos(pct * 2 * pi);
		yAxis = m_size * sin(pct * 2 * pi);
		arr[6 * i + 3] = m_center[0] + xAxis * right[0] + yAxis * forward[0];
		arr[6 * i + 4] = m_center[1] + xAxis * right[1] + yAxis * forward[1];
		arr[6 * i + 5] = m_center[2] + xAxis * right[2] + yAxis * forward[2];
	}
	return mem::shared_array(arr, 6 * m_sides, "Mesh");
}

shared_ptr<float> CircleMesh::_getTexCoords()
{
	float *arr = new float[6 * m_sides];
	for (int i = 0; i < 6 * m_sides; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr, 6 * m_sides, "Mesh");
}

shared_ptr<float> CircleMesh::_getNormals()
{
	float *arr = new float[6 * m_sides];
	for (int i = 0; i < 6 * m_sides; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr, 6 * m_sides, "Mesh");
}

shared_ptr<int> CircleMesh::_getIndices()
{
	int *arr = new int[2 * m_sides];
	for (int i = 0; i < 2 * m_sides; i++)
	{
		arr[i] = i;
	}
	return mem::shared_array(arr, 2 * m_sides, "Mesh");
}