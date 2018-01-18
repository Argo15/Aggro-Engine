#include "VertexBufferCache.hpp"
#include "DefaultVertexBufferHandle.hpp"
#include "FileBackedMesh.hpp"

VertexBufferCache::VertexBufferCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources)
	: m_graphics(graphics)
	, m_jobManager(jobManager)
	, m_resources(resources)
	, m_meshImporter(new AssimpMeshImporter())
	, m_idToVbo()
	, m_loadingVbos()
	, m_loadingSingleVbos()
	, m_defaultVbo(new DefaultVertexBufferHandle())
{
}

shared_ptr<VertexBufferHandle> VertexBufferCache::getVertexBuffer(shared_ptr<Mesh> mesh)
{
	// Id < 0 should be loaded immediately
	if (mesh->getId() < 0)
	{
		shared_ptr<LoadableVertexBufferHandle> loadingVbo(new LoadableVertexBufferHandle(m_defaultVbo));
		m_jobManager->addGraphicsJob(shared_ptr<Job>(new Job([this, mesh]()
		{
			shared_ptr<VertexBufferHandle> newVbo = m_graphics->createVertexBuffer(mesh);
			finishLoading(mesh, newVbo);
		})));
		m_loadingSingleVbos[mesh] = loadingVbo;
		return loadingVbo;
	}
	
	// Meshes with ids can be cached
	if (m_idToVbo.find(mesh->getId()) == m_idToVbo.end())
	{
		// If mesh already in memory, just create a VBO
		if (mesh->isResolved())
		{
			m_jobManager->addGraphicsJob(shared_ptr<Job>(new Job([this, mesh]()
			{
				shared_ptr<VertexBufferHandle> newVbo = m_graphics->createVertexBuffer(mesh);
				finishLoading(mesh->getId(), newVbo);
			})));
		}
		// If mesh not loaded in memory, do that in thread first then load.
		else
		{
			m_jobManager->add(shared_ptr<Job>(new Job([this, mesh]()
			{
				if (mesh->resolve())
				{
					m_jobManager->addGraphicsJob(shared_ptr<Job>(new Job([this, mesh]()
					{
						shared_ptr<VertexBufferHandle> newVbo = m_graphics->createVertexBuffer(mesh);
						finishLoading(mesh->getId(), newVbo);
					})));
				}
				else
				{
					failLoading(mesh->getId());
				}
			})));
		}
		shared_ptr<LoadableVertexBufferHandle> loadingVbo(new LoadableVertexBufferHandle(m_defaultVbo));
		m_loadingVbos[mesh->getId()] = loadingVbo;
		m_idToVbo[mesh->getId()] = loadingVbo;
	}

	return m_idToVbo[mesh->getId()];
}


void VertexBufferCache::finishLoading(int meshId, shared_ptr<VertexBufferHandle> handle)
{
	if (m_loadingVbos.find(meshId) != m_loadingVbos.end())
	{
		m_loadingVbos[meshId]->setHandle(handle);
		m_loadingVbos.erase(meshId);
	}
}

void VertexBufferCache::finishLoading(shared_ptr<Mesh> mesh, shared_ptr<VertexBufferHandle> handle)
{
	if (m_loadingSingleVbos.find(mesh) != m_loadingSingleVbos.end())
	{
		m_loadingSingleVbos[mesh]->setHandle(handle);
		m_loadingSingleVbos.erase(mesh);
	}
}

void VertexBufferCache::failLoading(int meshId)
{
	m_loadingVbos.erase(meshId);
}

void VertexBufferCache::failLoading(shared_ptr<Mesh> mesh)
{
	m_loadingSingleVbos.erase(mesh);
}