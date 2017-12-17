#include "ByteParser.hpp"
#include "Chunk.hpp"
#include "MemoryUtil.hpp"
#include <iostream>

ByteParser::ByteParser(int numBytes, char *bytes)
	: m_curIdx(0)
	, m_numBytes(numBytes)
	, m_bytes(bytes)
{

}

ByteParser::ByteParser(Chunk &chunk)
	: m_curIdx(0)
	, m_numBytes(*chunk.getNumBytes())
	, m_bytes(chunk.getByteData().get())
{

}

boost::optional<bool> ByteParser::parseBool()
{
	char cVal = parseChar().get_value_or('X');
	if ('X' == cVal)
	{
		return boost::optional<bool>();
	}
	return boost::optional<bool>('Y' == cVal);
}

boost::optional<char> ByteParser::parseChar()
{
	if (m_curIdx + sizeof(char) <= m_numBytes)
	{
		char val;
		memcpy(&val, m_bytes + m_curIdx, sizeof(char));
		m_curIdx += sizeof(char);
		return boost::optional<char>(val);
	}
	return boost::optional<char>();
}

boost::optional<short> ByteParser::parseShort()
{
	if (m_curIdx + sizeof(short) <= m_numBytes)
	{
		short val;
		memcpy(&val, m_bytes + m_curIdx, sizeof(short));
		m_curIdx += sizeof(short);
		return boost::optional<short>(val);
	}
	return boost::optional<short>();
}

boost::optional<int> ByteParser::parseInt()
{
	if (m_curIdx + sizeof(int) <= m_numBytes)
	{
		int val;
		memcpy(&val, m_bytes + m_curIdx, sizeof(int));
		m_curIdx += sizeof(int);
		return boost::optional<int>(val);
	}
	return boost::optional<int>();
}

boost::optional<unsigned int> ByteParser::parseUnsignedInt()
{
	if (m_curIdx + sizeof(unsigned int) <= m_numBytes)
	{
		unsigned int val;
		memcpy(&val, m_bytes + m_curIdx, sizeof(unsigned int));
		m_curIdx += sizeof(unsigned int);
		return boost::optional<unsigned int>(val);
	}
	return boost::optional<unsigned int>();
}

boost::optional<long> ByteParser::parseLong()
{
	if (m_curIdx + sizeof(long) <= m_numBytes)
	{
		long val;
		memcpy(&val, m_bytes + m_curIdx, sizeof(long));
		m_curIdx += sizeof(long);
		return boost::optional<long>(val);
	}
	return boost::optional<long>();
}

boost::optional<float> ByteParser::parseFloat()
{
	if (m_curIdx + sizeof(float) <= m_numBytes)
	{
		float val;
		memcpy(&val, m_bytes + m_curIdx, sizeof(float));
		m_curIdx += sizeof(float);
		return boost::optional<float>(val);
	}
	return boost::optional<float>();
}

boost::optional<double> ByteParser::parseDouble()
{
	if (m_curIdx + sizeof(double) <= m_numBytes)
	{
		double val;
		memcpy(&val, m_bytes + m_curIdx, sizeof(double));
		m_curIdx += sizeof(double);
		return boost::optional<double>(val);
	}
	return boost::optional<double>();
}

boost::optional<char *> ByteParser::parseBytes(int numBytes)
{
	if (m_curIdx + numBytes <= m_numBytes)
	{
		char *val = new char[numBytes];
		memcpy(val, m_bytes + m_curIdx, numBytes);
		m_curIdx += numBytes;
		return boost::optional<char *>(val);
	}
	return boost::optional<char *>();
}

boost::optional<Chunk> ByteParser::parseChunk()
{
	boost::optional<int> type = parseInt();
	boost::optional<int> numBytes = parseInt();
	if (type )
	{
		if (numBytes)
		{
			if (boost::optional<char *> bytes = parseBytes(*numBytes))
			{
				return boost::optional<Chunk>(Chunk((ChunkType)type.get(), *numBytes, mem::shared_array(*bytes)));
			}
		}
		return boost::optional<Chunk>(Chunk((ChunkType)type.get(), 0, shared_ptr<char>()));
	}
	return boost::optional<Chunk>();
}

boost::optional<string> ByteParser::parseString()
{
	if (boost::optional<Chunk> chunk = parseChunk())
	{
		if (*chunk->getType() == ChunkType::STRING)
		{
			return boost::optional<string>(string(chunk->getByteData().get(), *chunk->getNumBytes()));
		}
	}
	return boost::optional<string>();
}

boost::optional<glm::mat4> ByteParser::parseMat4()
{
	int numBytes = 16 * sizeof(float);
	if (boost::optional<char *> bytes = parseBytes(numBytes))
	{
		return boost::optional<glm::mat4>(glm::make_mat4((float *)*bytes));
	}
	return boost::optional<glm::mat4>();
}

boost::optional<glm::vec3> ByteParser::parseVec3()
{
	int numBytes = 3 * sizeof(float);
	if (boost::optional<char *> bytes = parseBytes(numBytes))
	{
		return boost::optional<glm::vec3>(glm::make_vec3((float *)*bytes));
	}
	return boost::optional<glm::vec3>();
}

boost::optional<glm::vec4> ByteParser::parseVec4()
{
	int numBytes = 4 * sizeof(float);
	if (boost::optional<char *> bytes = parseBytes(numBytes))
	{
		return boost::optional<glm::vec4>(glm::make_vec4((float *)*bytes));
	}
	return boost::optional<glm::vec4>();
}