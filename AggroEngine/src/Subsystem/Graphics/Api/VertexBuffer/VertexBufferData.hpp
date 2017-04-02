#pragma once

#include <memory>
using namespace std;

/**
 * Data used to build a vertex buffer object such as.
 *
 * author: wcrane
 * since: 2013-11-03
**/
class VertexBufferData
{
private:
	unsigned int m_nSizeOfVerticies;
	unsigned int m_nSizeOfIndicies;

	shared_ptr<float> m_nVerticies;
	shared_ptr<float> m_nTexCoords;
	shared_ptr<float> m_nNormals;
	shared_ptr<int> m_nIndicies;

public:
	VertexBufferData(int nSizeOfVerticies);
	VertexBufferData(int nSizeOfVerticies, int nSizeOfIndicies);

	VertexBufferData &setVerticies(shared_ptr<float> nVerticies);
	VertexBufferData &setTexCoords(shared_ptr<float> nTexCoords);
	VertexBufferData &setNormals(shared_ptr<float> nNormals);
	VertexBufferData &setIndicies(shared_ptr<int> nIndicies);
	
	unsigned int getSizeOfVerticies() const;
	unsigned int getSizeOfIndicies() const;
	const shared_ptr<float> getVerticies() const;
	const shared_ptr<float> getTexCoords() const;
	const shared_ptr<float> getNormals() const;
	const shared_ptr<int> getIndicies() const;
};