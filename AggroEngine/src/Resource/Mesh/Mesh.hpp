#pragma once

#include <memory>
#include <boost/optional.hpp>
using namespace std;

/**
 * Holds mesh data
 *
 * author: wcrane
 * since: 2014-01-25
**/
class Mesh
{
private:
	unsigned int m_nSizeOfVertices;
	unsigned int m_nSizeOfIndices;

	shared_ptr<float> m_npVertices;
	shared_ptr<float> m_npTexCoords;
	shared_ptr<float> m_npNormals;
	shared_ptr<int> m_npIndices;

protected:
	int m_meshId;

public:
	Mesh(int meshId);
	Mesh(int meshId, int nSizeOfVertices, int nSizeOfIndices, shared_ptr<float> npVertices, shared_ptr<float> npTexCoords, shared_ptr<float> npNormals, shared_ptr<int> npIndices);

	virtual bool resolve();
	virtual bool isResolved();

	int getId();

	void setSizeOfVerticies(unsigned int sizeOfVerticies);
	void setSizeOfIndicies(unsigned int sizeOfIndicies);
	void setVerticies(shared_ptr<float> verticies);
	void setTexCoords(shared_ptr<float> texCoords);
	void setNormals(shared_ptr<float> normals);
	void setIndicies(shared_ptr<int> indicies);

	unsigned int getSizeOfVerticies();
	unsigned int getSizeOfIndicies();
	shared_ptr<float> getVerticies();
	shared_ptr<float> getTexCoords();
	shared_ptr<float> getNormals();
	shared_ptr<int> getIndicies();

	virtual boost::optional<string> getMeshPath();

};