#include "Mesh.hpp"
#include <iostream>

Mesh::Mesh(int nSizeOfVerticies, int nSizeOfIndicies, shared_ptr<float> npVertices, shared_ptr<float> npTexCoords, shared_ptr<float> npNormals, shared_ptr<int> npIndices)
	: m_nSizeOfVertices(nSizeOfVerticies)
	, m_nSizeOfIndices(nSizeOfIndicies)
	, m_npVertices(npVertices)
	, m_npTexCoords(npTexCoords)
	, m_npNormals(npNormals)
	, m_npIndices(npIndices)
{
}

unsigned int Mesh::getSizeOfVerticies()
{
	return m_nSizeOfVertices;
}

unsigned int Mesh::getSizeOfIndicies()
{
	return m_nSizeOfIndices;
}

shared_ptr<float> Mesh::getVerticies()
{
	return m_npVertices;
}

shared_ptr<float> Mesh::getTexCoords()
{
	return m_npTexCoords;
}

shared_ptr<float> Mesh::getNormals()
{
	return m_npNormals;
}

shared_ptr<int> Mesh::getIndicies()
{
	return m_npIndices;
}