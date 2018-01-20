#include "MeshComponent.hpp"
#include "FileBackedMesh.hpp"

MeshComponent::MeshComponent()
	: m_primaryMesh()
{

}

MeshComponent::MeshComponent(Chunk * const byteChunk, shared_ptr<Resources> resources, shared_ptr<MeshCache> cache)
	: m_primaryMesh()
{
	ByteParser bytes = ByteParser(*byteChunk);
	string path = bytes.parseString().get_value_or("");
	if (path != "")
	{
		int id = resources->getIdForPath(path);
		cache->getMesh(id)->onReady([this](auto newMesh) {m_primaryMesh = newMesh; });
	}
}

shared_ptr<Chunk> MeshComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes = ByteAccumulator();
	string path = resources->getPathForId(m_primaryMesh->getId()).get_value_or("");
	bytes.add(&path);
	return shared_ptr<Chunk>(new Chunk(ChunkType::MESH_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<MeshComponent> MeshComponent::deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources, shared_ptr<MeshCache> cache)
{
	if (*byteChunk->getType() != ChunkType::MESH_COMPONENT)
	{
		return shared_ptr<MeshComponent>();
	}

	return shared_ptr<MeshComponent>(new MeshComponent(byteChunk, resources, cache));
}

void MeshComponent::addChangeListener(void *ns, std::function<void(MeshComponent *)> listener)
{
	boost::lock_guard<MeshComponent> guard(*this);
	m_changeListeners.add(ns, listener);
}

void MeshComponent::removeChangeListener(void *ns)
{
	boost::lock_guard<MeshComponent> guard(*this);
	m_changeListeners.remove(ns);
}

void MeshComponent::setPrimaryMesh(shared_ptr<Mesh> mesh)
{
	m_primaryMesh = mesh;
	m_changeListeners.notify(this);
}

shared_ptr<Mesh> MeshComponent::getPrimaryMesh()
{
	return m_primaryMesh;
}

void MeshComponent::clearMeshes()
{
	m_primaryMesh = shared_ptr<Mesh>();
	m_changeListeners.notify(this);
}

bool MeshComponent::hasMesh()
{
	return m_primaryMesh != nullptr;
}