#include "MeshMetaData.hpp"
#include "MemoryUtil.hpp"
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

	glm::vec3 *pCorners = new glm::vec3[8];
	pCorners[0] = glm::vec3(vMin.x, vMin.y, vMin.z);
	pCorners[1] = glm::vec3(vMin.x, vMin.y, vMax.z);
	pCorners[2] = glm::vec3(vMin.x, vMax.y, vMin.z);
	pCorners[3] = glm::vec3(vMin.x, vMax.y, vMax.z);
	pCorners[4] = glm::vec3(vMax.x, vMin.y, vMin.z);
	pCorners[5] = glm::vec3(vMax.x, vMin.y, vMax.z);
	pCorners[6] = glm::vec3(vMax.x, vMax.y, vMin.z);
	pCorners[7] = glm::vec3(vMax.x, vMax.y, vMax.z);
	m_corners = mem::shared_array(pCorners);
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

shared_ptr<glm::vec3> MeshMetaData::getCorners()
{
	return m_corners;
}