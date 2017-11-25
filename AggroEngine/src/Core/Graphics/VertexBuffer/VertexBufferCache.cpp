#include "VertexBufferCache.hpp"
#include "DefaultVertexBufferHandle.hpp"

VertexBufferCache::VertexBufferCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources)
	: m_graphics(graphics)
	, m_jobManager(jobManager)
	, m_resources(resources)
	, m_meshImporter(new AssimpMeshImporter())
	, m_pathsToVbo()
	, m_loadingVbos()
	, m_defaultVbo(new DefaultVertexBufferHandle())
{
}

shared_ptr<VertexBufferHandle> VertexBufferCache::getVertexBuffer(int meshId)
{
	boost::optional<string> meshPathOpt = m_resources->getPathForId(meshId);
	if (!meshPathOpt)
	{
		cout << "unknown mesh id " << meshId << endl;
		return m_defaultVbo;
	}
	const string meshPath = meshPathOpt.get();

	// Load vertex buffer if not loaded
	if (m_pathsToVbo.find(meshId) == m_pathsToVbo.end())
	{
		m_jobManager->add(shared_ptr<Job>(new Job([this, meshId, meshPath]()
		{
			shared_ptr<Mesh> mesh(m_meshImporter->importMesh(meshPath));
			if (mesh)
			{
				m_jobManager->addGraphicsJob(shared_ptr<Job>(new Job([this, mesh, meshId]()
				{
					shared_ptr<VertexBufferHandle> newVbo = m_graphics->createVertexBuffer(mesh);
					finishLoading(meshId, newVbo);
				})));
			}
			else
			{
				failLoading(meshId);
			}
		})));
		shared_ptr<LoadableVertexBufferHandle> loadingVbo(new LoadableVertexBufferHandle(m_defaultVbo));
		m_loadingVbos[meshId] = loadingVbo;
		m_pathsToVbo[meshId] = loadingVbo;
	}

	return m_pathsToVbo[meshId];
}

void VertexBufferCache::finishLoading(int meshId, shared_ptr<VertexBufferHandle> handle)
{
	if (m_loadingVbos.find(meshId) != m_loadingVbos.end())
	{
		m_loadingVbos[meshId]->setHandle(handle);
		m_loadingVbos.erase(meshId);
	}
}

void VertexBufferCache::failLoading(int meshId)
{
	m_loadingVbos.erase(meshId);
}