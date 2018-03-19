#include "BoundingBoxMesh.hpp"
#include "MemoryUtil.hpp"

static const int s_numVerts = 8;

BoundingBoxMesh::BoundingBoxMesh(int meshId, shared_ptr<glm::vec3> corners)
	: Mesh(meshId)
	, m_corners(corners)
{
	setSizeOfVerticies(sizeof(float) * 3 * s_numVerts);
	setSizeOfIndicies(sizeof(int) * 24);
	setVerticies(_getVertices());
	setTexCoords(_getTexCoords());
	setNormals(_getNormals());
	setIndicies(_getIndices());
	m_drawMode = DrawMode::LINES;
}

shared_ptr<float> BoundingBoxMesh::_getVertices()
{
	float *arr = new float[3 * s_numVerts];
	for (int i = 0; i < s_numVerts; i++)
	{
		arr[3 * i] = m_corners.get()[i].x;
		arr[3 * i + 1] = m_corners.get()[i].y;
		arr[3 * i + 2] = m_corners.get()[i].z;
	}
	return mem::shared_array(arr);
}

shared_ptr<float> BoundingBoxMesh::_getTexCoords()
{
	float *arr = new float[3 * s_numVerts];
	for (int i = 0; i < 3 * s_numVerts; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr);
}

shared_ptr<float> BoundingBoxMesh::_getNormals()
{
	float *arr = new float[3 * s_numVerts];
	for (int i = 0; i < 3 * s_numVerts; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr);
}

shared_ptr<int> BoundingBoxMesh::_getIndices()
{
	int *arr = new int[24];
	// Front
	arr[0] = 0;
	arr[1] = 1;
	arr[2] = 0;
	arr[3] = 2;
	arr[4] = 3;
	arr[5] = 1;
	arr[6] = 3;
	arr[7] = 2;
	// Across
	arr[8] = 0;
	arr[9] = 4;
	arr[10] = 1;
	arr[11] = 5;
	arr[12] = 2;
	arr[13] = 6;
	arr[14] = 3;
	arr[15] = 7;
	// Back
	arr[16] = 4;
	arr[17] = 5;
	arr[18] = 4;
	arr[19] = 6;
	arr[20] = 7;
	arr[21] = 5;
	arr[22] = 7;
	arr[23] = 6;
	return mem::shared_array(arr);
}