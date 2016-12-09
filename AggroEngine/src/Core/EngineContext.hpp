#pragma once

#include "OpenGL43Graphics.hpp"
#include "Scene.hpp"
#include "VertexBufferCache.hpp"
#include "TextureCache.hpp"
#include "JobManager.hpp"

class Scene;
class JobManager;

class EngineContext
{
private:
	shared_ptr<Graphics> m_graphics;
	shared_ptr<VertexBufferCache> m_vboCache;
	shared_ptr<TextureCache> m_textureCache;
	shared_ptr<Scene> m_scene;
	shared_ptr<RenderOptions> m_renderOptions;
	shared_ptr<JobManager> m_jobManager;

public:
	EngineContext();

	shared_ptr<Graphics> getGraphics();
	shared_ptr<VertexBufferCache> getVboCache();
	shared_ptr<TextureCache> getTextureCache();
	shared_ptr<Scene> getScene();
	shared_ptr<RenderOptions> getRenderOptions();
	shared_ptr<JobManager> getJobManager();
};