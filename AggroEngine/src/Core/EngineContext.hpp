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
	shared_ptr<GraphicsContext> m_graphicsContext;
	shared_ptr<Scene> m_scene;
	shared_ptr<RenderOptions> m_renderOptions;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;

public:
	EngineContext();

	shared_ptr<GraphicsContext> getGraphicsContext();
	shared_ptr<Graphics> getGraphics();
	shared_ptr<VertexBufferCache> getVboCache();
	shared_ptr<TextureCache> getTextureCache();
	shared_ptr<Scene> getScene();
	shared_ptr<RenderOptions> getRenderOptions();
	shared_ptr<JobManager> getJobManager();
	shared_ptr<Resources> getResources();
};