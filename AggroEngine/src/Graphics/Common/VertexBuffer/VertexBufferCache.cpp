#include "VertexBufferCache.hpp"

VertexBufferCache::VertexBufferCache(shared_ptr<Graphics> graphics)
	: m_graphics(graphics)
	, m_meshImporter(shared_ptr<MeshImporter>(new AssimpMeshImporter()))
	, m_pathsToVbo()
{

}

shared_ptr<VertexBufferHandle> VertexBufferCache::getVertexBuffer(string sFilename)
{
	if (m_pathsToVbo.find(sFilename) == m_pathsToVbo.end())
	{
		shared_ptr<Mesh> mesh(m_meshImporter->importMesh(sFilename));
		shared_ptr<VertexBufferHandle> newVbo = m_graphics->createVertexBuffer(mesh);
		m_pathsToVbo[sFilename] = newVbo;
	}
	return m_pathsToVbo[sFilename];
}