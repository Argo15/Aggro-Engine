#include "MeshComponent.hpp"
#include "FileBackedMesh.hpp"
#include "GenerateTangents.hpp"
#include "AlignCenter.hpp"
#include "AlignBottom.hpp"
#include "BoundingBoxMod.hpp"

static shared_ptr<MeshModifier> normalLineGen(new GenerateNormalLines());
static shared_ptr<MeshModifier> tangentGen(new GenerateTangents());
static shared_ptr<MeshModifier> alignCenter(new AlignCenter());
static shared_ptr<MeshModifier> alignBottom(new AlignBottom());
static shared_ptr<MeshModifier> boundingBoxGen(new BoundingBoxMod());

MeshComponent::MeshComponent(shared_ptr<JobManager> jobs)
	: m_jobs(jobs)
	, m_primaryMesh()
	, m_genTangents(true)
	, m_genNormalLines(false)
	, m_genBoundingBox(false)
	, m_axisAlign(NONE)
{
}

MeshComponent::~MeshComponent()
{
	lock();
	unlock();
}

MeshComponent::MeshComponent(Chunk * const byteChunk, 
							 shared_ptr<Resources> resources, 
							 shared_ptr<MeshCache> cache, 
							 shared_ptr<JobManager> jobs)
	: m_jobs(jobs)
	, m_primaryMesh()
	, m_genTangents(true)
{
	ByteParser bytes = ByteParser(*byteChunk);
	string path = bytes.parseString().get_value_or("");
	m_genNormalLines = bytes.parseBool().get_value_or(false);
	m_axisAlign = (AxisAlign)bytes.parseInt().get_value_or(0);
	m_genBoundingBox = bytes.parseBool().get_value_or(false);

	if (path != "")
	{
		int id = resources->getIdForPath(path);
		cache->getMesh(id)->onReady([this, jobs](auto newMesh) {
			m_primaryMesh = newMesh;
			generateMeshes();
		});
	}
}

shared_ptr<Chunk> MeshComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes = ByteAccumulator();
	string path = m_primaryMesh ? resources->getPathForId(m_primaryMesh->getId()).get_value_or("") : "";
	bytes.add(&path);
	bytes.add(&m_genNormalLines);
	int axis = (int) m_axisAlign;
	bytes.add(&axis);
	bytes.add(&m_genBoundingBox);
	return shared_ptr<Chunk>(new Chunk(ChunkType::MESH_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<MeshComponent> MeshComponent::deserialize(Chunk * const byteChunk, 
												 	 shared_ptr<Resources> resources, 
													 shared_ptr<MeshCache> cache, 
													 shared_ptr<JobManager> jobs)
{
	if (*byteChunk->getType() != ChunkType::MESH_COMPONENT)
	{
		return shared_ptr<MeshComponent>();
	}

	return shared_ptr<MeshComponent>(new MeshComponent(byteChunk, resources, cache, jobs));
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
	boost::lock_guard<MeshComponent> guard(*this);
	if (m_primaryMesh)
	{
		m_modifiedPrimaryMesh = m_primaryMesh;
		m_jobs->add(shared_ptr<Job>(new Job([this]() {
			if (!m_modifiedPrimaryMesh)
			{
				return;
			}
			auto tracker = PerfStats::instance().trackTime("generateMeshes");
			switch (m_axisAlign)
			{
				case AxisAlign::CENTER: m_modifiedPrimaryMesh = alignCenter->apply(m_modifiedPrimaryMesh); break;
				case AxisAlign::BOTTOM: m_modifiedPrimaryMesh = alignBottom->apply(m_modifiedPrimaryMesh); break;
				default: break;
			}
			if (m_genTangents)
			{
				m_modifiedPrimaryMesh = tangentGen->apply(m_modifiedPrimaryMesh);
			}
			if (m_genNormalLines && !m_normalLines)
			{
				m_normalLines = normalLineGen->apply(m_modifiedPrimaryMesh);
			}
			if (m_genBoundingBox && !m_boundingBox)
			{
				m_boundingBox = boundingBoxGen->apply(m_modifiedPrimaryMesh);
			}
			refresh();
		})), JobPriority::HIGH);
	}
}

void MeshComponent::refresh()
{
	boost::lock_guard<MeshComponent> guard(*this);
	m_modifiedMeshes = vector<shared_ptr<Mesh>>();
	if (m_primaryMesh)
	{
		m_modifiedMeshes.push_back(m_modifiedPrimaryMesh);
		if (m_genNormalLines)
		{
			m_modifiedMeshes.push_back(m_normalLines);
		}
		if (m_genBoundingBox)
		{
			m_modifiedMeshes.push_back(m_boundingBox);
		}
	}
}

void MeshComponent::enableTangents(bool enabled, bool generate)
{
	m_genTangents = enabled;
	if (generate)
	{
		generateMeshes();
	}
}

bool MeshComponent::isTangentsEnabled()
{
	return m_genTangents;
}

void MeshComponent::enableNormalLines(bool enabled, bool generate)
{
	m_genNormalLines = enabled;
	if (generate)
	{
		generateMeshes();
	}
	if (!enabled)
	{
		refresh();
	}
}

bool MeshComponent::isNormalLinesEnabled()
{
	return m_genNormalLines;
}

void MeshComponent::setAxisAlign(AxisAlign align)
{
	m_axisAlign = align;
	generateMeshes();
	m_changeListeners.notify(this);
}

MeshComponent::AxisAlign MeshComponent::getAxisAlign()
{
	return m_axisAlign;
}

void MeshComponent::enableBoundingBox(bool enabled, bool generate)
{
	m_genBoundingBox = enabled;
	if (generate)
	{
		generateMeshes();
	}
	if (!enabled)
	{
		refresh();
	}
}

bool MeshComponent::isBoundingBoxEnabled()
{
	return m_genBoundingBox;
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

bool MeshComponent::modsReady(shared_ptr<VertexBufferCache> vbos)
{
	if (m_modifiedMeshes.empty())
	{
		return false;
	}
	shared_ptr<VertexBufferHandle> vbo = vbos->getVertexBuffer(m_modifiedMeshes[0]);
	return vbo && vbo->isLoaded();
}

glm::vec3 MeshComponent::getMeshCenter()
{
	if (!m_modifiedPrimaryMesh)
	{
		return glm::vec3(0);
	}
	return m_modifiedPrimaryMesh->getMetaData()->getCenter();
}
