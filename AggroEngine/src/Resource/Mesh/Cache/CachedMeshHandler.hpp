#pragma once

#include "MeshImporter.hpp"
#include "Listener.hpp"
#include <memory>
using namespace std;

class CachedMeshHandler
{
private:
	shared_ptr<Mesh> m_mesh;
	Listener<shared_ptr<Mesh>> m_readyListeners;

public:
	CachedMeshHandler(shared_ptr<Mesh> mesh = nullptr);

	shared_ptr<Mesh> get();
	void updateMesh(shared_ptr<Mesh> mesh);

	void notifyReady();
	void onReady(function<void(shared_ptr<Mesh>)> func);
};