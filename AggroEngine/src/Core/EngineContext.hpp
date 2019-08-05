#pragma once

#include "GraphicsContext.hpp"
#include "Scene.hpp"
#include "JobManager.hpp"
#include "Resources.hpp"
#include "MeshCache.hpp"
#include "SceneNodeCache.hpp"

class Scene;
class JobManager;
class SceneNodeCache;

class EngineContext
{
private:
	shared_ptr<Scene> m_scene;
	shared_ptr<RenderOptions> m_renderOptions;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;
	shared_ptr<MeshCache> m_meshCache;
	shared_ptr<SceneNodeCache> m_sceneNodeCache;

	Listener<Scene *> m_newSceneListeners;

public:
	EngineContext();

	shared_ptr<Scene> getScene();
	shared_ptr<RenderOptions> getRenderOptions();
	shared_ptr<JobManager> getJobManager();
	shared_ptr<Resources> getResources();
	shared_ptr<MeshCache> getMeshCache();
	shared_ptr<SceneNodeCache> getSceneNodeCache();

	void setScene(shared_ptr<Scene> scene);
	void addNewSceneListener(function<void(Scene *)> func);
};