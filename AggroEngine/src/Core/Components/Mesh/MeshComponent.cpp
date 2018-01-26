#include "MeshComponent.hpp"
#include "FileBackedMesh.hpp"

static shared_ptr<MeshModifier> normalLineGen(new GenerateNormalLines());

MeshComponent::MeshComponent()
	: m_primaryMesh()
	, m_genNormalLines(false)
{
}

MeshComponent::MeshComponent(Chunk * const byteChunk, shared_ptr<Resources> resources, shared_ptr<MeshCache> cache)
	: m_primaryMesh()
{
	ByteParser bytes = ByteParser(*byteChunk);
	string path = bytes.parseString().get_value_or("");
	m_genNormalLines = bytes.parseBool().get_value_or(false);
	if (path != "")
	{
		int id = resources->getIdForPath(path);
		cache->getMesh(id)->onReady([this](auto newMesh) {
			m_primaryMesh = newMesh;
			generateMeshes();
		});
	}
}

shared_ptr<Chunk> MeshComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes = ByteAccumulator();
	string path = resources->getPathForId(m_primaryMesh->getId()).get_value_or("");
	bytes.add(&path);
	bytes.add(&m_genNormalLines);
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

void MeshComponent::generateMeshes()
{
	if (m_primaryMesh)
	{
		generateNormalLines();
	}
	refresh();
}

void MeshComponent::refresh()
{
	m_modifiedMeshes = vector<shared_ptr<Mesh>>();
	if (m_primaryMesh)
	{
		m_modifiedMeshes.push_back(m_primaryMesh);
		if (m_genNormalLines)
		{
			m_modifiedMeshes.push_back(m_normalLines);
		}
	}
}

void MeshComponent::generateNormalLines()
{
	m_normalLines = m_genNormalLines ? normalLineGen->apply(m_primaryMesh) : shared_ptr<Mesh>();
}

void MeshComponent::enableNormalLines(bool enabled, bool generate)
{
	m_genNormalLines = enabled;
	if (generate)
	{
		generateNormalLines();
	}
	refresh();
}

bool MeshComponent::isNormalLinesEnabled()
{
	return m_genNormalLines;
}

void MeshComponent::setPrimaryMesh(shared_ptr<Mesh> mesh)
{
	m_primaryMesh = mesh;
	generateMeshes();
	m_changeListeners.notify(this);
}

shared_ptr<Mesh> MeshComponent::getPrimaryMesh()
{
	return m_primaryMesh;
}

void MeshComponent::clearMeshes()
{
	m_primaryMesh = shared_ptr<Mesh>();
	generateMeshes();
	m_changeListeners.notify(this);
}

vector<shared_ptr<Mesh>> &MeshComponent::getModifiedMeshes()
{
	return m_modifiedMeshes;
}

bool MeshComponent::hasMesh()
{
	return m_primaryMesh != nullptr;
}