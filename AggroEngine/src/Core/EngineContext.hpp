#pragma once

#include "GraphicsContext.hpp"
#include "Scene.hpp"
#include "JobManager.hpp"
#include "Resources.hpp"

class Scene;
class JobManager;

class EngineContext
{
private:
	shared_ptr<Scene> m_scene;
	shared_ptr<RenderOptions> m_renderOptions;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;

	Listener<Scene *> m_newSceneListeners;

public:
	EngineContext();

	shared_ptr<Scene> getScene();
	shared_ptr<RenderOptions> getRenderOptions();
	shared_ptr<JobManager> getJobManager();
	shared_ptr<Resources> getResources();

	void setScene(shared_ptr<Scene> scene);
	void addNewSceneListener(function<void(Scene *)> func);
};