#include "EngineContext.hpp"

EngineContext::EngineContext()
	: m_jobManager(new JobManager())
	, m_scene(new Scene())
	, m_renderOptions(new RenderOptions())
	, m_resources(new Resources())
	, m_meshImporter(new AssimpMeshImporter())
	, m_newSceneListeners()
{
}

shared_ptr<Scene> EngineContext::getScene()
{
	return m_scene;
}

shared_ptr<RenderOptions> EngineContext::getRenderOptions()
{
	return m_renderOptions;
}

shared_ptr<JobManager> EngineContext::getJobManager()
{
	return m_jobManager;
}

shared_ptr<Resources> EngineContext::getResources()
{
	return m_resources;
}

shared_ptr<MeshImporter> EngineContext::getMeshImporter()
{
	return m_meshImporter;
}

void EngineContext::setScene(shared_ptr<Scene> scene)
{
	m_scene = scene;
	m_newSceneListeners.notify(m_scene.get());
}

void EngineContext::addNewSceneListener(function<void(Scene *)> listner)
{
	m_newSceneListeners.add(listner);
}