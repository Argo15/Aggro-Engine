#pragma once

#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
#include "LoadableVertexBufferHandle.hpp"
#include "AssimpMeshImporter.hpp"
#include "Graphics.hpp"
#include "JobManager.hpp"
using namespace std;

class VertexBufferCache
{
private:
	shared_ptr<MeshImporter> m_meshImporter;
	shared_ptr<Graphics> m_graphics;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<VertexBufferHandle> m_defaultVbo;

	boost::unordered_map<string, shared_ptr<VertexBufferHandle>> m_pathsToVbo;
	boost::unordered_map<string, shared_ptr<LoadableVertexBufferHandle>> m_loadingVbos;

public:
	VertexBufferCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager);

	shared_ptr<VertexBufferHandle> getVertexBuffer(string sFilename);

	void finishLoading(string sFilename, shared_ptr<VertexBufferHandle> handle);
};