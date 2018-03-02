#include "MeshMetaData.hpp"
#include <algorithm>
using namespace std;

MeshMetaData::MeshMetaData(Mesh *mesh)
{
	m_numTriangles = mesh->getSizeOfIndicies() / 12;
	unsigned int numVerts = mesh->getSizeOfVerticies() / 12; // xyz * 4 bytes per float
	shared_ptr<float> verts = mesh->getVerticies();

	glm::vec3 vMax(-999999999.f);
	glm::vec3 vMin(999999999.f);
	for (int i = 0; i < numVerts; i++)
	{
		vMax.x = max(vMax.x, verts.get()[3 * i]);
		vMax.y = max(vMax.y, verts.get()[3 * i + 1]);
		vMax.z = max(vMax.z, verts.get()[3 * i + 2]);
		vMin.x = min(vMin.x, verts.get()[3 * i]);
		vMin.y = min(vMin.y, verts.get()[3 * i + 1]);
		vMin.z = min(vMin.z, verts.get()[3 * i + 2]);
	}
	m_center = (vMax + vMin) / 2.0f;
	m_xLength = abs(vMax.x - vMin.x);
	m_yLength = abs(vMax.y - vMin.y);
	m_zLength = abs(vMax.z - vMin.z);
}

int MeshMetaData::getNumTriangles()
{
	return m_numTriangles;
}

glm::vec3 &MeshMetaData::getCenter()
{
	return m_center;
}

float MeshMetaData::getXLength()
{
	return m_xLength;
}

float MeshMetaData::getYLength()
{
	return m_yLength;
}

float MeshMetaData::getZLength()
{
	return m_zLength;
}