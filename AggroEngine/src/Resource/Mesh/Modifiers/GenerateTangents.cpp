#include "GenerateTangents.hpp"
#include "LineMesh.hpp"
#include "MemoryUtil.hpp"

static glm::vec3 top = glm::vec3(0, 1.0, 0);
static glm::vec3 side = glm::vec3(1.0, 0, 0);

GenerateTangents::GenerateTangents()
{

}

shared_ptr<Mesh> GenerateTangents::apply(shared_ptr<Mesh> &sourceMesh)
{
	if (!sourceMesh || sourceMesh->getSizeOfVerticies() == 0)
	{
		return shared_ptr<Mesh>();
	}
	unsigned int sizeOfVertices = sourceMesh->getSizeOfVerticies();
	unsigned int sizeOfIndices = sourceMesh->getSizeOfIndicies();
	shared_ptr<float> originalVerts = sourceMesh->getVerticies();
	shared_ptr<float> originalTexCoords = sourceMesh->getTexCoords();
	shared_ptr<float> originalNorms = sourceMesh->getNormals();
	shared_ptr<int> originalIndices = sourceMesh->getIndicies();
	shared_ptr<Mesh> newMesh = shared_ptr<Mesh>(new Mesh(
		-1,
		sizeOfVertices,
		sizeOfIndices,
		originalVerts,
		originalTexCoords,
		originalNorms,
		originalIndices,
		DrawMode::TRIANGLES));

	int numNormals = sizeOfVertices / 12; // 12 bytes (xyz floats) per vertex
	int numTriangles = sizeOfIndices / 12; // 12 bytes (3 ints) per triangle
	float *tangents = new float[3 * numNormals]; // xyz
	float *bitangents = new float[3 * numNormals]; // xyz

	// Init
	for (int i = 0; i < 3 * numNormals; i++)
	{
		tangents[i] = 0;
		bitangents[i] = 0;
	}

	for (int i = 0; i < numTriangles; i++)
	{
		// Triangle indicies
		int idx0 = originalIndices.get()[3 * i];
		int idx1 = originalIndices.get()[3 * i + 1];
		int idx2 = originalIndices.get()[3 * i + 2];

		// Verticies
		glm::vec3 v0(
			originalVerts.get()[3 * idx0],
			originalVerts.get()[3 * idx0 + 1],
			originalVerts.get()[3 * idx0 + 2]
		);
		glm::vec3 v1(
			originalVerts.get()[3 * idx1],
			originalVerts.get()[3 * idx1 + 1],
			originalVerts.get()[3 * idx1 + 2]
		);
		glm::vec3 v2(
			originalVerts.get()[3 * idx2],
			originalVerts.get()[3 * idx2 + 1],
			originalVerts.get()[3 * idx2 + 2]
		);

		// UVs
		glm::vec2 uv0(
			originalTexCoords.get()[2 * idx0],
			originalTexCoords.get()[2 * idx0 + 1]
		);
		glm::vec2 uv1(
			originalTexCoords.get()[2 * idx1],
			originalTexCoords.get()[2 * idx1 + 1]
		);
		glm::vec2 uv2(
			originalTexCoords.get()[2 * idx2],
			originalTexCoords.get()[2 * idx2 + 1]
		);

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = glm::normalize(deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
		glm::vec3 bitangent = glm::normalize(deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);

		tangents[3 * idx0] += tangent.x;
		tangents[3 * idx0 + 1] += tangent.y;
		tangents[3 * idx0 + 2] += tangent.z;
		tangents[3 * idx1] += tangent.x;
		tangents[3 * idx1 + 1] += tangent.y;
		tangents[3 * idx1 + 2] += tangent.z;
		tangents[3 * idx2] += tangent.x;
		tangents[3 * idx2 + 1] += tangent.y;
		tangents[3 * idx2 + 2] += tangent.z;

		bitangents[3 * idx0] += bitangent.x;
		bitangents[3 * idx0 + 1] += bitangent.y;
		bitangents[3 * idx0 + 2] += bitangent.z;
		bitangents[3 * idx1] += bitangent.x;
		bitangents[3 * idx1 + 1] += bitangent.y;
		bitangents[3 * idx1 + 2] += bitangent.z;
		bitangents[3 * idx2] += bitangent.x;
		bitangents[3 * idx2 + 1] += bitangent.y;
		bitangents[3 * idx2 + 2] += bitangent.z;
	}

	// Normalize
	for (int i = 0; i < numNormals; i++)
	{
		glm::vec3 tangent(tangents[3 * i], tangents[3 * i + 1], tangents[3 * i + 2]);
		tangent = glm::normalize(tangent);
		tangents[3 * i] = tangent.x;
		tangents[3 * i + 1] = tangent.y;
		tangents[3 * i + 2] = tangent.z;

		glm::vec3 bitangent(bitangents[3 * i], bitangents[3 * i + 1], bitangents[3 * i + 2]);
		bitangent = glm::normalize(bitangent);
		bitangents[3 * i] = bitangent.x;
		bitangents[3 * i + 1] = bitangent.y;
		bitangents[3 * i + 2] = bitangent.z;
	}
	newMesh->setTangents(mem::shared_array(tangents));
	newMesh->setBitangents(mem::shared_array(bitangents));
	return newMesh;
}