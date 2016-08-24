#pragma once

#include "VertexBufferHandle.hpp"
#include "AssimpMeshImporter.hpp"
#include "Graphics.hpp"
#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
using namespace std;

class VertexBufferCache
{
private:
	shared_ptr<MeshImporter> m_meshImporter;
	shared_ptr<Graphics> m_graphics;

	boost::unordered_map<string, shared_ptr<VertexBufferHandle>> m_pathsToVbo;

public:
	VertexBufferCache(shared_ptr<Graphics> graphics);

	shared_ptr<VertexBufferHandle> getVertexBuffer(string sFilename);
};