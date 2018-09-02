#include "GenerateNormalLines.hpp"
#include "LineMesh.hpp"
#include "MemoryUtil.hpp"

GenerateNormalLines::GenerateNormalLines()
{

}

shared_ptr<Mesh> GenerateNormalLines::apply(shared_ptr<Mesh> &sourceMesh)
{
	if (!sourceMesh)
	{
		return shared_ptr<Mesh>();
	}
	unsigned int sizeOfVertices = sourceMesh->getSizeOfVerticies();
	unsigned int sizeOfIndices = sourceMesh->getSizeOfIndicies();
	int numNormals = sizeOfVertices / 12; // 12 bytes per vertex
	int vertsPerNormal = sourceMesh->hasTangents() ? 6 : 2; // one line or three?

	int numVertFloats = 3 * vertsPerNormal; // XYZ for each
	int numTexFloats = 2 * vertsPerNormal; // UV for each
	int numIndexInts = vertsPerNormal; // 1 for each

	float *verticies = new float[numVertFloats * numNormals];
	float *texCoords = new float[numTexFloats * numNormals];
	float *normals = new float[numVertFloats * numNormals];
	int *indicies = new int[numIndexInts * numNormals];
	float *originalVerts = sourceMesh->getVerticies().get();
	float *originalNorms = sourceMesh->getNormals().get();
	float *originalTang = sourceMesh->getTangents().get();
	float *originalBitang = sourceMesh->getBitangents().get();
	for (int i = 0; i < numNormals; i++)
	{
		verticies[numVertFloats * i] = originalVerts[3 * i];
		verticies[numVertFloats * i + 1] = originalVerts[3 * i + 1];
		verticies[numVertFloats * i + 2] = originalVerts[3 * i + 2];

		verticies[numVertFloats * i + 3] = originalVerts[3 * i] + originalNorms[3 * i] * 0.1;
		verticies[numVertFloats * i + 4] = originalVerts[3 * i + 1] + originalNorms[3 * i + 1] * 0.1;
		verticies[numVertFloats * i + 5] = originalVerts[3 * i + 2] + originalNorms[3 * i + 2] * 0.1;

		if (sourceMesh->hasTangents())
		{
			verticies[numVertFloats * i + 6] = originalVerts[3 * i];
			verticies[numVertFloats * i + 7] = originalVerts[3 * i + 1];
			verticies[numVertFloats * i + 8] = originalVerts[3 * i + 2];

			verticies[numVertFloats * i + 9] = originalVerts[3 * i] + originalTang[3 * i] * 0.1;
			verticies[numVertFloats * i + 10] = originalVerts[3 * i + 1] + originalTang[3 * i + 1] * 0.1;
			verticies[numVertFloats * i + 11] = originalVerts[3 * i + 2] + originalTang[3 * i + 2] * 0.1;

			verticies[numVertFloats * i + 12] = originalVerts[3 * i];
			verticies[numVertFloats * i + 13] = originalVerts[3 * i + 1];
			verticies[numVertFloats * i + 14] = originalVerts[3 * i + 2];

			verticies[numVertFloats * i + 15] = originalVerts[3 * i] + originalBitang[3 * i] * 0.1;
			verticies[numVertFloats * i + 16] = originalVerts[3 * i + 1] + originalBitang[3 * i + 1] * 0.1;
			verticies[numVertFloats * i + 17] = originalVerts[3 * i + 2] + originalBitang[3 * i + 2] * 0.1;
		}

		for (int x = 0; x < numTexFloats; x++)
		{
			texCoords[numTexFloats * i + x] = 0;
		}
		for (int x = 0; x < numVertFloats; x++)
		{
			normals[numVertFloats * i + x] = 0;
		}
		for (int x = 0; x < numIndexInts; x++)
		{
			indicies[numIndexInts * i + x] = numIndexInts * i + x;
		}
	}
	shared_ptr<float> pVertices = mem::shared_array(verticies, numVertFloats * numNormals, "Mesh");
	shared_ptr<float> pTexCoords = mem::shared_array(texCoords, numTexFloats * numNormals, "Mesh");
	shared_ptr<float> pNormals = mem::shared_array(normals, numVertFloats * numNormals, "Mesh");
	shared_ptr<int> pIndices = mem::shared_array(indicies, numIndexInts * numNormals, "Mesh");
	return shared_ptr<Mesh>(new Mesh(
		-1, 
		numVertFloats * numNormals * sizeof(float),
		numIndexInts * numNormals * sizeof(int),
		pVertices, 
		pTexCoords,
		pNormals,
		pIndices,
		DrawMode::LINES));
}