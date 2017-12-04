#pragma once

#include <string>
#include "Chunk.hpp"
using namespace std;

class FileWriter 
{
public:
	static void writeToFile(string filename, shared_ptr<Chunk> chunk);
	static shared_ptr<Chunk> readFile(string filename);
};