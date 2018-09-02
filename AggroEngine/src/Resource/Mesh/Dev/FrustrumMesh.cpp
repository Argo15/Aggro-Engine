#include "FrustrumMesh.hpp"
#include "MemoryUtil.hpp"
#include "Matrix.hpp"

const static int s_numVerts = 8;
const static int s_numIdx = 24; // 12 lines

FrustrumMesh::FrustrumMesh(int meshId, shared_ptr<Frustrum> frustrum)
	: Mesh(meshId)
	, m_frustrum(frustrum)
{
	setSizeOfVerticies(sizeof(float) * 3 * s_numVerts);
	setSizeOfIndicies(sizeof(int) * s_numIdx);
	setVerticies(_getVertices());
	setTexCoords(_getTexCoords());
	setNormals(_getNormals());
	setIndicies(_getIndices());
}

shared_ptr<float> FrustrumMesh::_getVertices()
{
	shared_ptr<glm::vec3> corners = m_frustrum->getCorners();
	float *arr = new float[s_numVerts * 3];
	for (int i = 0; i < s_numVerts; i++)
	{
		arr[3 * i + 0] = corners.get()[i].x;
		arr[3 * i + 1] = corners.get()[i].y;
		arr[3 * i + 2] = corners.get()[i].z;
	}
	return mem::shared_array(arr, s_numVerts * 3, "Mesh");
}

shared_ptr<float> FrustrumMesh::_getTexCoords()
{
	float *arr = new float[s_numVerts * 3];
	for (int i = 0; i < s_numVerts * 3; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr, s_numVerts * 3, "Mesh");
}

shared_ptr<float> FrustrumMesh::_getNormals()
{
	float *arr = new float[s_numVerts * 3];
	for (int i = 0; i < s_numVerts * 3; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr, s_numVerts * 3, "Mesh");
}

shared_ptr<int> FrustrumMesh::_getIndices()
{
	int *arr = new int[s_numIdx];
	// near
	arr[0] = 0;
	arr[1] = 1;
	arr[2] = 0;
	arr[3] = 2;
	arr[4] = 1;
	arr[5] = 3;
	arr[6] = 2;
	arr[7] = 3;
	// far
	arr[8] = 4;
	arr[9] = 5;
	arr[10] = 4;
	arr[11] = 6;
	arr[12] = 5;
	arr[13] = 7;
	arr[14] = 6;
	arr[15] = 7;
	// between
	arr[16] = 0;
	arr[17] = 4;
	arr[18] = 1;
	arr[19] = 5;
	arr[20] = 2;
	arr[21] = 6;
	arr[22] = 3;
	arr[23] = 7;
	return mem::shared_array(arr, s_numIdx, "Mesh");
}