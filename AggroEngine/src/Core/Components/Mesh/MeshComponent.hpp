#pragma once

#include "Locks.hpp"
#include "Component.hpp"
#include "Mesh.hpp"
#include "Serialization.hpp"
#include "Resources.hpp"
#include "MeshCache.hpp"
#include "Listener.hpp"
#include <vector>
using namespace std;

/**
* Holds Mesh and Mesh Modifiers
*
* author: wcrane
* since: 2017-12-22
**/
class MeshComponent : public Component,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<Mesh> m_primaryMesh;

	Listener<MeshComponent *> m_changeListeners;

	MeshComponent(Chunk * const byteChunk, shared_ptr<Resources> resources, shared_ptr<MeshCache> cache);

public:
	MeshComponent();

	shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<MeshComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources, shared_ptr<MeshCache> cache);

	void addChangeListener(void *ns, std::function<void(MeshComponent *)> listener);
	void removeChangeListener(void *ns);

	void setPrimaryMesh(shared_ptr<Mesh> mesh);
	shared_ptr<Mesh> getPrimaryMesh();
	void clearMeshes();

	bool hasMesh();
};