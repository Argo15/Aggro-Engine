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

	glm::vec3 vMax(-999999999.f);
	glm::vec3 vMin(999999999.f);
	for (int i = 0; i < numVerts; i++)
	{
		vMax.x = max(vMax.x, originalVerts.get()[3 * i]);
		vMax.y = max(vMax.y, originalVerts.get()[3 * i + 1]);
		vMax.z = max(vMax.z, originalVerts.get()[3 * i + 2]);
		vMin.x = min(vMin.x, originalVerts.get()[3 * i]);
		vMin.y = min(vMin.y, originalVerts.get()[3 * i + 1]);
		vMin.z = min(vMin.z, originalVerts.get()[3 * i + 2]);
	}
	glm::vec3 vAvg = (vMax + vMin) / 2.0f;

	float *newVerts = new float[3 * numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		newVerts[3 * i] = originalVerts.get()[3 * i] - vAvg.x;
		newVerts[3 * i + 1] = originalVerts.get()[3 * i + 1] - vAvg.y;
		newVerts[3 * i + 2] = originalVerts.get()[3 * i + 2] - vAvg.z;
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