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
	int numVerts = sizeOfVertices / 12; // 3 x 4 bytes each
	float *verticies = new float[6 * numVerts];
	float *texCoords = new float[4 * numVerts];
	float *normals = new float[6 * numVerts];
	int *indicies = new int[2 * numVerts];
	float *originalVerts = sourceMesh->getVerticies().get();
	float *originalNorms = sourceMesh->getNormals().get();
	for (int i = 0; i < numVerts; i++)
	{
		verticies[6 * i] = originalVerts[3 * i];
		verticies[6 * i + 1] = originalVerts[3 * i + 1];
		verticies[6 * i + 2] = originalVerts[3 * i + 2];

		verticies[6 * i + 3] = originalVerts[3 * i] + originalNorms[3 * i] * 0.1;
		verticies[6 * i + 4] = originalVerts[3 * i + 1] + originalNorms[3 * i + 1] * 0.1;
		verticies[6 * i + 5] = originalVerts[3 * i + 2] + originalNorms[3 * i + 2] * 0.1;

		for (int x = 0; x < 4; x++)
		{
			texCoords[4 * i + x] = 0;
		}
		for (int x = 0; x < 6; x++)
		{
			normals[6 * i + x] = 0;
		}
		indicies[2 * i] = 2 * i;
		indicies[2 * i + 1] = 2 * i + 1;
	}
	shared_ptr<float> pVertices = mem::shared_array(verticies);
	shared_ptr<float> pTexCoords = mem::shared_array(texCoords);
	shared_ptr<float> pNormals = mem::shared_array(normals);
	shared_ptr<int> pIndices = mem::shared_array(indicies);
	return shared_ptr<Mesh>(new Mesh(
		-1, 
		6 * numVerts * sizeof(float),
		2 * numVerts * sizeof(int),
		pVertices, 
		pTexCoords,
		pNormals,
		pIndices,
		DrawMode::LINES));
}