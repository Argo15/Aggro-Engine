#pragma once

#include "MeshImporter.hpp"
#include "Resources.hpp"
#include "Mesh.hpp"
#include "Locks.hpp"

/**
* A mesh that will lazy load from a file.
*
* author: wcrane
* since: 2018-01-14
**/
class FileBackedMesh : public Mesh
{
private:
	shared_ptr<Resources> m_resources;
	shared_ptr<MeshImporter> m_importer;
	boost::optional<string> m_filename;
	bool m_resolved;
	bool m_loaded;

public:
	FileBackedMesh(int meshId, shared_ptr<Resources> resources, shared_ptr<MeshImporter> importer);

	virtual bool resolve();
	virtual bool isResolved();

	virtual boost::optional<string> getMeshPath();
};