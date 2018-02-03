#pragma once

#include "Locks.hpp"
#include "Component.hpp"
#include "Mesh.hpp"
#include "Serialization.hpp"
#include "Resources.hpp"
#include "MeshCache.hpp"
#include "Listener.hpp"
#include "GenerateNormalLines.hpp"
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
	shared_ptr<Mesh> m_modifiedPrimaryMesh;
	bool m_genTangents;

	bool m_genNormalLines;
	shared_ptr<Mesh> m_normalLines;

	vector<shared_ptr<Mesh>> m_modifiedMeshes;

	Listener<MeshComponent *> m_changeListeners;

	MeshComponent(Chunk * const byteChunk, shared_ptr<Resources> resources, shared_ptr<MeshCache> cache);

public:
	MeshComponent();

	shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<MeshComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources, shared_ptr<MeshCache> cache);

	void addChangeListener(void *ns, std::function<void(MeshComponent *)> listener);
	void removeChangeListener(void *ns);

	void generateMeshes();
	void refresh();

	void enableTangents(bool enabled, bool generate = false);
	bool isTangentsEnabled();

	void generateNormalLines();
	void enableNormalLines(bool enabled, bool generate = false);
	bool isNormalLinesEnabled();

	void setPrimaryMesh(shared_ptr<Mesh> mesh);
	shared_ptr<Mesh> getPrimaryMesh();
	void clearMeshes();

	vector<shared_ptr<Mesh>> &getModifiedMeshes();

	bool hasMesh();
};