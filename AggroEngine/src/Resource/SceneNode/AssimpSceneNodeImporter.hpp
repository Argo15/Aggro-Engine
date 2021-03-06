#pragma once

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SceneNodeImporter.hpp"
#include "Locks.hpp"

class AssimpSceneNodeImporter : public SceneNodeImporter
	, public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	Assimp::Importer importer;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;

	shared_ptr<MeshComponent> readMesh(aiMesh *meshIn);
	void readMaterial(string relativePath, aiMaterial *matIn, MaterialComponent *matOut);

public:
	AssimpSceneNodeImporter(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources);

	shared_ptr<SceneNode> importSceneNode(string sFilename);
};