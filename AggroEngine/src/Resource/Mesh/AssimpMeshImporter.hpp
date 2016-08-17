#pragma once

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshImporter.hpp"

class AssimpMeshImporter : public MeshImporter
{
private:
  Assimp::Importer importer;

public:
	AssimpMeshImporter();

	shared_ptr<Mesh> importMesh(string sFilename);
};