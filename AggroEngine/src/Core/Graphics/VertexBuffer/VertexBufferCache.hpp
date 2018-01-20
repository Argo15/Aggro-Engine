#pragma once

#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
#include "LoadableVertexBufferHandle.hpp"
#include "AssimpMeshImporter.hpp"
#include "Graphics.hpp"
#include "JobManager.hpp"
#include "Resources.hpp"
using namespace std;

class VertexBufferCache
{
private:
	shared_ptr<Graphics> m_graphics;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;
	shared_ptr<VertexBufferHandle> m_defaultVbo;

	boost::unordered_map<int, shared_ptr<VertexBufferHandle>> m_idToVbo;
	boost::unordered_map<int, shared_ptr<LoadableVertexBufferHandle>> m_loadingVbos;

	// For meshes that don't have ids, such as the line grid in the editor.
	boost::unordered_map<shared_ptr<Mesh>, shared_ptr<LoadableVertexBufferHandle>> m_loadingSingleVbos;

public:
	VertexBufferCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources);

	shared_ptr<VertexBufferHandle> getVertexBuffer(shared_ptr<Mesh> mesh);
	void finishLoading(int meshId, shared_ptr<VertexBufferHandle> handle);
	void finishLoading(shared_ptr<Mesh> mesh, shared_ptr<VertexBufferHandle> handle);
	void failLoading(int meshId);
	void failLoading(shared_ptr<Mesh> mesh);
};