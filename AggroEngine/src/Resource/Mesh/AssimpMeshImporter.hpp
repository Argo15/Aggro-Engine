#pragma once

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshImporter.hpp"
#include "Locks.hpp"

class AssimpMeshImporter : public MeshImporter
	, public boost::basic_lockable_adapter<recursive_mutex>
{
private:
  Assimp::Importer importer;

public:
	AssimpMeshImporter();

	shared_ptr<Mesh> importMesh(string sFilename);
};