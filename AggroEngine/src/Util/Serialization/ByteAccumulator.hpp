#pragma once

#include "Chunk.hpp"
#include "Matrix.hpp"
#include <vector>
#include <memory>
using namespace std;

struct ByteItem
{
	int m_numBytes;
	const void *m_bytePtr;

	ByteItem(const int numBytes, const void *bytePtr)
	{
		m_numBytes = numBytes;
		m_bytePtr = bytePtr;
	}
};

/**
* Collects mutliple objects and converts it to a byte array
*
* author: wcrane
* since: 2017-11-25
**/
class ByteAccumulator
{
private:
	int m_numBytes;
	vector<ByteItem> m_byteItems;

public:
	ByteAccumulator();

	void add(const bool *value);
	void add(const char *value);
	void add(const short *value);
	void add(const int *value);
	void add(const unsigned int *value);
	void add(const long *value);
	void add(const float *value);
	void add(const double *value);
	void add(const int numBytes, const void *pBytes);
	void add(Chunk *chunk);
	void add(string *str);
	void add(glm::mat4 *mat);
	void add(glm::vec3 *vec);
	void add(glm::vec4 *vec);

	int getNumBytes();
	shared_ptr<char> collect();
};