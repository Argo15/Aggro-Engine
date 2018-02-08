#include "AlignBottom.hpp"
#include "Matrix.hpp"
#include "MemoryUtil.hpp"

AlignBottom::AlignBottom()
{

}

shared_ptr<Mesh> AlignBottom::apply(shared_ptr<Mesh> &sourceMesh)
{
	if (!sourceMesh)
	{
		return shared_ptr<Mesh>();
	}
	unsigned int numVerts = sourceMesh->getSizeOfVerticies() / 12; // xyz * 4 bytes per float
	shared_ptr<float> originalVerts = sourceMesh->getVerticies();

	float yBottom = 999999999.f;
	for (int i = 0; i < numVerts; i++)
	{
		yBottom = min(yBottom, originalVerts.get()[3 * i + 1]);
	}

	float *newVerts = new float[3 * numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		newVerts[3 * i] = originalVerts.get()[3 * i];
		newVerts[3 * i + 1] = originalVerts.get()[3 * i + 1] - yBottom;
		newVerts[3 * i + 2] = originalVerts.get()[3 * i + 2];
	}

	return shared_ptr<Mesh>(new Mesh(
		-1,
		sourceMesh->getSizeOfVerticies(),
		sourceMesh->getSizeOfIndicies(),
		mem::shared_array(newVerts),
		sourceMesh->getTexCoords(),
		sourceMesh->getNormals(),
		sourceMesh->getIndicies(),
		DrawMode::TRIANGLES));
}