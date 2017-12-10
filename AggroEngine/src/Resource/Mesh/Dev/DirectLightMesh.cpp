#include "DirectLightMesh.hpp"
#include "MemoryUtil.hpp"

const static int s_numVerts = 18;

DirectLightMesh::DirectLightMesh()
{
	setSizeOfVerticies(sizeof(float) * 3 * s_numVerts);
	setSizeOfIndicies(sizeof(int) * s_numVerts);
	setVerticies(_getVertices());
	setTexCoords(_getTexCoords());
	setNormals(_getNormals());
	setIndicies(_getIndices());
}

shared_ptr<float> DirectLightMesh::_getVertices()
{
	float *arr = new float[s_numVerts * 3];
	arr[0] = 0;
	arr[1] = -1.3;
	arr[2] = 0;
	arr[3] = 0;
	arr[4] = -4.0;
	arr[5] = 0;
	int numRemaining = (s_numVerts - 2) / 2;
	for (int i = 0; i < numRemaining; i++)
	{
		float rad = 6.2832 * (float) i / (float) numRemaining;
		float x = cos(rad);
		float y = sin(rad);
		arr[6 + 6 * i + 0] = 0.5 * x;
		arr[6 + 6 * i + 1] = -1.3;
		arr[6 + 6 * i + 2] = 0.5 * y;
		arr[6 + 6 * i + 3] = 0.5 * x;
		arr[6 + 6 * i + 4] = -4.0;
		arr[6 + 6 * i + 5] = 0.5 * y;
	}
	return mem::shared_array(arr);
}

shared_ptr<float> DirectLightMesh::_getTexCoords()
{
	float *arr = new float[s_numVerts * 3];
	for (int i = 0; i < s_numVerts * 3; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr);
}

shared_ptr<float> DirectLightMesh::_getNormals()
{
	float *arr = new float[s_numVerts * 3];
	for (int i = 0; i < s_numVerts * 3; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr);
}

shared_ptr<int> DirectLightMesh::_getIndices()
{
	int *arr = new int[s_numVerts];
	for (int i = 0; i < s_numVerts; i++)
	{
		arr[i] = i;
	}
	return mem::shared_array(arr);
}