#pragma once

#include <memory>
#include "Mesh.hpp"
#include "Matrix.hpp"
using namespace std;

class Mesh;

class MeshMetaData
{
private:
	int m_numTriangles;
	glm::vec3 m_center;
	float m_xLength;
	float m_yLength;
	float m_zLength;
	shared_ptr<glm::vec3> m_corners;
	float m_radius;

public:
	MeshMetaData(Mesh *mesh);

	int getNumTriangles();
	glm::vec3 &getCenter();
	float getXLength();
	float getYLength();
	float getZLength();
	float getRadius();

	shared_ptr<glm::vec3> getCorners();
};