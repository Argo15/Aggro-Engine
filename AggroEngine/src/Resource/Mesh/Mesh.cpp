#include "Mesh.hpp"
#include <iostream>

Mesh::Mesh(int meshId)
	: m_meshId(meshId)
	, m_drawMode(DrawMode::TRIANGLES)
	, m_nSizeOfVertices(0)
	, m_nSizeOfIndices(0)
{
}

Mesh::Mesh(int meshId, int nSizeOfVerticies, int nSizeOfIndicies, shared_ptr<float> npVertices, shared_ptr<float> npTexCoords, shared_ptr<float> npNormals, shared_ptr<int> npIndices, DrawMode drawMode)
	: m_meshId(meshId)
	, m_nSizeOfVertices(nSizeOfVerticies)
	, m_nSizeOfIndices(nSizeOfIndicies)
	, m_npVertices(npVertices)
	, m_npTexCoords(npTexCoords)
	, m_npNormals(npNormals)
	, m_npIndices(npIndices)
	, m_drawMode(drawMode)
{
}

Mesh::~Mesh()
{
	this->lock();
	this->unlock();
}

bool Mesh::resolve()
{
	return true;
}

bool Mesh::isResolved()
{
	return true;
}

int Mesh::getId()
{
	return m_meshId;
}

void Mesh::setSizeOfVerticies(unsigned int sizeOfVerticies)
{
	m_nSizeOfVertices = sizeOfVerticies;
}

void Mesh::setSizeOfIndicies(unsigned int sizeOfIndicies)
{
	m_nSizeOfIndices = sizeOfIndicies;
}

void Mesh::setVerticies(shared_ptr<float> verticies)
{
	m_npVertices = verticies;
}

void Mesh::setTexCoords(shared_ptr<float> texCoords)
{
	m_npTexCoords = texCoords;
}

void Mesh::setNormals(shared_ptr<float> normals)
{
	m_npNormals = normals;
}

void Mesh::setTangents(shared_ptr<float> tangents)
{
	m_npTangents = tangents;
}

void Mesh::setBitangents(shared_ptr<float> bitangents)
{
	m_npBitangents = bitangents;
}

void Mesh::setIndicies(shared_ptr<int> indicies)
{
	m_npIndices = indicies;
}

unsigned int Mesh::getSizeOfVerticies()
{
	resolve();
	return m_nSizeOfVertices;
}

unsigned int Mesh::getSizeOfIndicies()
{
	resolve();
	return m_nSizeOfIndices;
}

shared_ptr<float> Mesh::getVerticies()
{
	resolve();
	return m_npVertices;
}

shared_ptr<float> Mesh::getTexCoords()
{
	resolve();
	return m_npTexCoords;
}

shared_ptr<float> Mesh::getNormals()
{
	resolve();
	return m_npNormals;
}

shared_ptr<float> Mesh::getTangents()
{
	resolve();
	return m_npTangents;
}

shared_ptr<float> Mesh::getBitangents()
{
	resolve();
	return m_npBitangents;
}

shared_ptr<int> Mesh::getIndicies()
{
	resolve();
	return m_npIndices;
}

shared_ptr<MeshMetaData> Mesh::getMetaData()
{
	boost::lock_guard<Mesh> guard(*this);
	if (!m_metaData)
	{
		m_metaData = shared_ptr<MeshMetaData>(new MeshMetaData(this));
	}
	return m_metaData;
}

bool Mesh::hasTangents()
{
	return m_npTangents != nullptr && m_npBitangents != nullptr;
}

boost::optional<string> Mesh::getMeshPath()
{
	return boost::optional<string>();
}

DrawMode Mesh::getDrawMode()
{
	return m_drawMode;
}