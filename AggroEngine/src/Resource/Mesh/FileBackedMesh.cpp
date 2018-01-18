#include "FileBackedMesh.hpp"

FileBackedMesh::FileBackedMesh(int meshId, shared_ptr<Resources> resources, shared_ptr<MeshImporter> importer)
	: Mesh(meshId)
	, m_resolved(false)
	, m_loaded(false)
	, m_resources(resources)
	, m_importer(importer)
	, m_filename(resources->getPathForId(meshId))
{
}

bool FileBackedMesh::resolve()
{
	boost::lock_guard<FileBackedMesh> guard(*this);
	if (m_resolved)
	{
		return true;
	}
	m_resolved = true;

	if (m_filename)
	{
		shared_ptr<Mesh> mesh = m_importer->importMesh(m_meshId, *m_filename);
		if (!mesh)
		{
			return false;
		}
		this->setSizeOfVerticies(mesh->getSizeOfVerticies());
		setSizeOfIndicies(mesh->getSizeOfIndicies());
		setVerticies(mesh->getVerticies());
		setTexCoords(mesh->getTexCoords());
		setNormals(mesh->getNormals());
		setIndicies(mesh->getIndicies());
		m_loaded = true;
	}
	return true;
}

bool FileBackedMesh::isResolved()
{
	return m_resolved;
}

boost::optional<string> FileBackedMesh::getMeshPath()
{
	return m_filename;
}