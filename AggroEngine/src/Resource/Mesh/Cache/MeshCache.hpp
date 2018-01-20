#pragma once

#include "CachedMeshHandler.hpp"
#include "MeshImporter.hpp"
#include "JobManager.hpp"
#include "Resources.hpp"
#include "Locks.hpp"
#include <memory>
using namespace std;

class MeshCache : public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<MeshImporter> m_meshImporter;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;

	boost::unordered_map<int, shared_ptr<CachedMeshHandler>> m_idToMesh;

public:
	MeshCache(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources);

	shared_ptr<CachedMeshHandler> getMesh(int meshId);
};