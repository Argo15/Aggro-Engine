#include "VertexBufferCache.hpp"
#include "DefaultVertexBufferHandle.hpp"

VertexBufferCache::VertexBufferCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager)
	: m_graphics(graphics)
	, m_jobManager(jobManager)
	, m_meshImporter(shared_ptr<MeshImporter>(new AssimpMeshImporter()))
	, m_pathsToVbo()
	, m_loadingVbos()
	, m_defaultVbo(new DefaultVertexBufferHandle())
{

}

shared_ptr<VertexBufferHandle> VertexBufferCache::getVertexBuffer(string sFilename)
{
	// Load texture if not loaded
	if (m_pathsToVbo.find(sFilename) == m_pathsToVbo.end())
	{
		m_jobManager->add(shared_ptr<Job>(new Job([this, sFilename]()
		{
			shared_ptr<Mesh> mesh(m_meshImporter->importMesh(sFilename));
			m_jobManager->addGraphicsJob(shared_ptr<Job>(new Job([this, mesh, sFilename]()
			{
				shared_ptr<VertexBufferHandle> newVbo = m_graphics->createVertexBuffer(mesh);
				finishLoading(sFilename, newVbo);
			})));
		})));
		shared_ptr<LoadableVertexBufferHandle> loadingVbo(new LoadableVertexBufferHandle(m_defaultVbo));
		m_loadingVbos[sFilename] = loadingVbo;
		m_pathsToVbo[sFilename] = loadingVbo;
	}

	return m_pathsToVbo[sFilename];
}

void VertexBufferCache::finishLoading(string sFilename, shared_ptr<VertexBufferHandle> handle)
{
	if (m_loadingVbos.find(sFilename) != m_loadingVbos.end())
	{
		m_loadingVbos[sFilename]->setHandle(handle);
		m_loadingVbos.erase(sFilename);
	}
}