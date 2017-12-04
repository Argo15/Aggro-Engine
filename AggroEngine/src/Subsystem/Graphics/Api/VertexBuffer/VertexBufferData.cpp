#include "VertexBufferData.hpp"
#include "MemoryUtil.hpp"
#include <iostream>

VertexBufferData::VertexBufferData(int nSizeOfVerticies)
	: m_nSizeOfVerticies(nSizeOfVerticies)
	, m_nSizeOfIndicies(nSizeOfVerticies/3)
{
	unsigned int nNumIndicies = m_nSizeOfIndicies/sizeof(int);
	int *nTmpIndicies = new int[nNumIndicies];
	for (unsigned int i=0; i<nNumIndicies; i++)
	{
		nTmpIndicies[i] = i;
	}
	m_nIndicies = mem::shared_array(nTmpIndicies);
}

VertexBufferData::VertexBufferData(int nSizeOfVerticies, int nSizeOfIndicies)
	: m_nSizeOfVerticies(nSizeOfVerticies)
	, m_nSizeOfIndicies(nSizeOfIndicies)
{
}

VertexBufferData &VertexBufferData::setVerticies(shared_ptr<float> nVerticies)
{
	m_nVerticies = nVerticies;
	return *this;
}

VertexBufferData &VertexBufferData::setTexCoords(shared_ptr<float> nTexCoords)
{
	m_nTexCoords = nTexCoords;
	return *this;
}

VertexBufferData &VertexBufferData::setNormals(shared_ptr<float> nNormals)
{
	m_nNormals = nNormals;
	return *this;
}

VertexBufferData &VertexBufferData::setIndicies(shared_ptr<int> nIndicies)
{
	m_nIndicies = nIndicies;
	return *this;
}

unsigned int VertexBufferData::getSizeOfVerticies() const
{
	return m_nSizeOfVerticies;
}

unsigned int VertexBufferData::getSizeOfIndicies() const
{
	return m_nSizeOfIndicies;
}

const shared_ptr<float> VertexBufferData::getVerticies() const
{
	return m_nVerticies;
}

const shared_ptr<float> VertexBufferData::getTexCoords() const
{
	return m_nTexCoords;
}

const shared_ptr<float> VertexBufferData::getNormals() const
{
	return m_nNormals;
}

const shared_ptr<int> VertexBufferData::getIndicies() const
{
	return m_nIndicies;
}