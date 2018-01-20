#include "MeshCache.hpp"
#include "AssimpMeshImporter.hpp"
#include "FileBackedMesh.hpp"

MeshCache::MeshCache(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources)
	: m_meshImporter(new AssimpMeshImporter())
	, m_jobManager(jobManager)
	, m_resources(resources)
	, m_idToMesh()
{

}

shared_ptr<CachedMeshHandler> MeshCache::getMesh(int meshId)
{
	if (m_idToMesh.find(meshId) == m_idToMesh.end())
	{
		m_idToMesh[meshId] = shared_ptr<CachedMeshHandler>(new CachedMeshHandler());
		m_jobManager->add(shared_ptr<Job>(new Job([this, meshId]()
		{
			shared_ptr<Mesh> newMesh = shared_ptr<Mesh>(new FileBackedMesh(meshId, m_resources, m_meshImporter));
			newMesh->resolve();
			m_idToMesh[meshId]->updateMesh(newMesh);
			m_idToMesh[meshId]->notifyReady();
		})));
	}
	return m_idToMesh[meshId];
}