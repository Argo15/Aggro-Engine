#pragma once

#include <memory>
#include <string>
#include "Mesh.hpp"
using namespace std;

class MeshImporter
{
public:
	virtual shared_ptr<Mesh> importMesh(string sFilename) = 0;
};