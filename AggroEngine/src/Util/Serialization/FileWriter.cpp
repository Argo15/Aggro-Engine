#include "FileWriter.hpp"
#include "MemoryUtil.hpp"
#include <iostream>
#include <fstream>

void FileWriter::writeToFile(string filename, shared_ptr<Chunk> chunk)
{
	if (chunk)
	{
		ofstream file(filename, ios::out | ios::binary | ios::ate);
		if (file.is_open())
		{
			file.write((char*)chunk->getType(), sizeof(ChunkType));
			file.write((char*)chunk->getNumBytes(), sizeof(int));
			file.write(chunk->getByteData().get(), *chunk->getNumBytes());
			file.close();
		}
	}
}

shared_ptr<Chunk> FileWriter::readFile(string filename)
{
	ifstream file(filename, ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		file.seekg(0, ios::beg);
		ChunkType chunkType;
		int numBytes;
		file.read((char*)&chunkType, sizeof(ChunkType));
		file.read((char*)&numBytes, sizeof(int));
		char *byteData = new char[numBytes];
		file.read(byteData, numBytes);
		file.close();
		return shared_ptr<Chunk>(new Chunk(chunkType, numBytes, mem::shared_array(byteData, numBytes, "Serializer")));
	}
	return shared_ptr<Chunk>();
}