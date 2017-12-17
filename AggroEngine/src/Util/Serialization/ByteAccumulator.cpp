#include "ByteAccumulator.hpp"
#include "MemoryUtil.hpp"

static char *boolTrue = new char('Y');
static char *boolFalse = new char('N');

ByteAccumulator::ByteAccumulator()
	: m_numBytes(0)
	, m_byteItems()
{
}

void ByteAccumulator::add(const bool *value)
{
	add(*value ? boolTrue : boolFalse);
}

void ByteAccumulator::add(const char *value)
{
	m_numBytes += sizeof(char);
	m_byteItems.push_back(ByteItem(sizeof(char), value));
}

void ByteAccumulator::add(const short *value)
{
	m_numBytes += sizeof(short);
	m_byteItems.push_back(ByteItem(sizeof(short), value));
}

void ByteAccumulator::add(const int *value)
{
	m_numBytes += sizeof(int);
	m_byteItems.push_back(ByteItem(sizeof(int), value));
}

void ByteAccumulator::add(const unsigned int *value)
{
	m_numBytes += sizeof(unsigned int);
	m_byteItems.push_back(ByteItem(sizeof(unsigned int), value));
}

void ByteAccumulator::add(const long *value)
{
	m_numBytes += sizeof(long);
	m_byteItems.push_back(ByteItem(sizeof(long), value));
}

void ByteAccumulator::add(const float *value)
{
	m_numBytes += sizeof(float);
	m_byteItems.push_back(ByteItem(sizeof(float), value));
}

void ByteAccumulator::add(const double *value)
{
	m_numBytes += sizeof(double);
	m_byteItems.push_back(ByteItem(sizeof(double), value));
}

void ByteAccumulator::add(const int numBytes, const void *pBytes)
{
	m_numBytes += numBytes;
	m_byteItems.push_back(ByteItem(numBytes, pBytes));
}

void ByteAccumulator::add(Chunk *chunk)
{
	m_numBytes += *chunk->getNumBytes();
	add((int *)chunk->getType());
	add(chunk->getNumBytes());
	m_byteItems.push_back(ByteItem(*chunk->getNumBytes(), chunk->getByteData().get()));
}

void ByteAccumulator::add(string *str)
{
	ByteAccumulator bytes = ByteAccumulator();
	int len = strlen(str->c_str());
	char *cstr = new char[len];
	std::strcpy(cstr, str->c_str());
	add(new Chunk(ChunkType::STRING, len, mem::shared_array(cstr)));
}

void ByteAccumulator::add(glm::mat4 *mat)
{
	float *values = glm::value_ptr(*mat);
	add(16 * sizeof(float), values);
}

void ByteAccumulator::add(glm::vec3 *vec)
{
	float *values = glm::value_ptr(*vec);
	add(3 * sizeof(float), values);
}

void ByteAccumulator::add(glm::vec4 *vec)
{
	float *values = glm::value_ptr(*vec);
	add(4 * sizeof(float), values);
}

int ByteAccumulator::getNumBytes()
{
	return m_numBytes;
}

shared_ptr<char> ByteAccumulator::collect()
{
	char *data = new char[m_numBytes];
	int curIdx = 0;
	for (auto & item : m_byteItems)
	{
		memcpy(data + curIdx, item.m_bytePtr, item.m_numBytes);
		curIdx += item.m_numBytes;
	}
	return mem::shared_array(data);
}
