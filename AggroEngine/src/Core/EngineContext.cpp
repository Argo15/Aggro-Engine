#include "EngineContext.hpp"

EngineContext::EngineContext()
	: m_jobManager(new JobManager())
	, m_scene(new Scene())
	, m_renderOptions(new RenderOptions())
	, m_resources(new Resources())
	, m_meshCache(new MeshCache(m_jobManager, m_resources))
	, m_sceneNodeCache(new SceneNodeCache(m_jobManager, m_resources))
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

shared_ptr<MeshCache> EngineContext::getMeshCache()
{
	return m_meshCache;
}

shared_ptr<SceneNodeCache> EngineContext::getSceneNodeCache()
{
	return m_sceneNodeCache;
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