#include "AlignCenter.hpp"
#include "Matrix.hpp"
#include "MemoryUtil.hpp"

AlignCenter::AlignCenter()
{

}

shared_ptr<Mesh> AlignCenter::apply(shared_ptr<Mesh> &sourceMesh)
{
	if (!sourceMesh)
	{
		return shared_ptr<Mesh>();
	}
	unsigned int numVerts = sourceMesh->getSizeOfVerticies() / 12; // xyz * 4 bytes per float
	shared_ptr<float> originalVerts = sourceMesh->getVerticies();

	glm::vec3 vCenter = sourceMesh->getMetaData()->getCenter();
	float *newVerts = new float[3 * numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		newVerts[3 * i] = originalVerts.get()[3 * i] - vCenter.x;
		newVerts[3 * i + 1] = originalVerts.get()[3 * i + 1] - vCenter.y;
		newVerts[3 * i + 2] = originalVerts.get()[3 * i + 2] - vCenter.z;
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