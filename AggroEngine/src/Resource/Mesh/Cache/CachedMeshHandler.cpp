#include "CachedMeshHandler.hpp"

CachedMeshHandler::CachedMeshHandler(shared_ptr<Mesh> mesh)
	: m_mesh(mesh)
{

}

shared_ptr<Mesh> CachedMeshHandler::get()
{
	return m_mesh;
}

void CachedMeshHandler::updateMesh(shared_ptr<Mesh> mesh)
{
	m_mesh = mesh;
}

void CachedMeshHandler::notifyReady()
{
	m_readyListeners.notify(m_mesh);
	m_readyListeners.remove();
}

void CachedMeshHandler::onReady(function<void(shared_ptr<Mesh>)> func)
{
	if (m_mesh)
	{
		func(m_mesh);
	}
	else
	{
		m_readyListeners.add(func);
	}
}