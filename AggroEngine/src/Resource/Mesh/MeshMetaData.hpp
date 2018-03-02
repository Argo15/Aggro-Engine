#pragma once

#include "Mesh.hpp"
#include "Matrix.hpp"

class Mesh;

class MeshMetaData
{
private:
	int m_numTriangles;
	glm::vec3 m_center;
	float m_xLength;
	float m_yLength;
	float m_zLength;

public:
	MeshMetaData(Mesh *mesh);

	int getNumTriangles();
	glm::vec3 &getCenter();
	float getXLength();
	float getYLength();
	float getZLength();
};