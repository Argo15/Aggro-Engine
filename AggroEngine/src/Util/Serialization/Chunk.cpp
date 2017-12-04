#include "Chunk.hpp"

Chunk::Chunk(ChunkType type, int numBytes, std::shared_ptr<char> data)
	: m_type(type)
	, m_numBytes(numBytes)
	, m_data(data) 
{
}

ChunkType *Chunk::getType()
{
	return &m_type;
}

int *Chunk::getNumBytes()
{
	return &m_numBytes;
}

std::shared_ptr<char> Chunk::getByteData()
{
	return m_data;
}