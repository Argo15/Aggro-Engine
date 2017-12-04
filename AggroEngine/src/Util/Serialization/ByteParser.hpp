#pragma once

#include <boost/optional.hpp>
#include <string>
#include "Chunk.hpp"
#include "Matrix.hpp"
using namespace std;

class ByteParser
{
private:
	int m_curIdx;
	int m_numBytes;
	char *m_bytes;

public:
	ByteParser(int numBytes, char *bytes);
	ByteParser(Chunk &chunk);

	boost::optional<bool> parseBool();
	boost::optional<char> parseChar();
	boost::optional<short> parseShort();
	boost::optional<int> parseInt();
	boost::optional<unsigned int> parseUnsignedInt();
	boost::optional<long> parseLong();
	boost::optional<float> parseFloat();
	boost::optional<double> parseDouble();
	boost::optional<char *> parseBytes(int numBytes);
	boost::optional<Chunk> parseChunk();
	boost::optional<string> parseString();
	boost::optional<glm::mat4> parseMat4();
	boost::optional<glm::vec3> parseVec3();
};