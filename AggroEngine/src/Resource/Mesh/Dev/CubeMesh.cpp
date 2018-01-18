#include "CubeMesh.hpp"
#include "MemoryUtil.hpp"
#include <iostream>

static const float verts[] = 
{
	// +Z
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	// +X
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 // +Y
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 // -Z
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 // -X
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	 // +Y
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
};

static const float texcoords[] = 
{
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
};

static const float normals[] = 
{
	// +Z 
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	// +X 
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	// +Y
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	// -Z
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	// -X 
    -1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	// -Y
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
};

static const int idxs[] = 
{
	// +Z
	0, 1, 2,
	3, 0, 2,
	// +X
	4, 5, 6,
	7, 4, 6,
	// +Y
	8, 10, 9,
	11, 10, 8,
	// -Z
	12, 14, 13,
	15, 14, 12,
	// -X
	16, 18, 17,
	19, 18, 16,
	// -Y
	20, 21, 22,
	23, 20, 22,
};

shared_ptr<float> CubeMesh::_getVertices()
{
	int length = sizeof(verts)/sizeof(float);
	float *pVerts = new float[length];
	for (int i=0; i<length; i++)
	{
		pVerts[i] = verts[i];
	}
	return mem::shared_array(pVerts);
}

shared_ptr<float> CubeMesh::_getTexCoords()
{
	int length = sizeof(texcoords)/sizeof(float);
	float *pTex = new float[length];
	for (int i=0; i<length; i++)
	{
		pTex[i] = texcoords[i];
	}
	return mem::shared_array(pTex);
}

shared_ptr<float> CubeMesh::_getNormals()
{
	int length = sizeof(normals)/sizeof(float);
	float *pNorms = new float[length];
	for (int i=0; i<length; i++)
	{
		pNorms[i] = normals[i];
	}
	return mem::shared_array(pNorms);
}

shared_ptr<int> CubeMesh::_getIndices()
{
	int length = sizeof(idxs)/sizeof(int);
	int *pIdx = new int[length];
	for (int i=0; i<length; i++)
	{
		pIdx[i] = idxs[i];
	}
	return mem::shared_array(pIdx);
}

CubeMesh::CubeMesh(int meshId)
	: Mesh(meshId, sizeof(verts), sizeof(idxs), _getVertices(), _getTexCoords(), _getNormals(), _getIndices())
{
}