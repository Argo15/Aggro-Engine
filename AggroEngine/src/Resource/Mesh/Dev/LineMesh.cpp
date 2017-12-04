#include "LineMesh.hpp"
#include "MemoryUtil.hpp"

LineMesh::LineMesh(glm::vec3 p1, glm::vec3 p2)
	: m_p1(p1)
	, m_p2(p2)
{
	setSizeOfVerticies(sizeof(float) * 6);
	setSizeOfIndicies(sizeof(int) * 2);
	setVerticies(_getVertices());
	setTexCoords(_getTexCoords());
	setNormals(_getNormals());
	setIndicies(_getIndices());
}

shared_ptr<float> LineMesh::_getVertices()
{
	float *arr = new float[6];
	arr[0] = m_p1[0];
	arr[1] = m_p1[1];
	arr[2] = m_p1[2];
	arr[3] = m_p2[0];
	arr[4] = m_p2[1];
	arr[5] = m_p2[2];
	return mem::shared_array(arr);
}

shared_ptr<float> LineMesh::_getTexCoords()
{
	float *arr = new float[6];
	for (int i = 0; i < 6; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr);
}

shared_ptr<float> LineMesh::_getNormals()
{
	float *arr = new float[6];
	for (int i = 0; i < 6; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr);
}

shared_ptr<int> LineMesh::_getIndices()
{
	int *arr = new int[2];
	arr[0] = 0;
	arr[1] = 1;
	return mem::shared_array(arr);
}