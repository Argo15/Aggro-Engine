#pragma once

#include "OpenGL43Graphics.hpp"
#include "Scene.hpp"
#include "VertexBufferCache.hpp"
#include "TextureCache.hpp"
#include "JobManager.hpp"

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
	int m_fps;

public:
	EngineContext();

	shared_ptr<Graphics> getGraphics();
	shared_ptr<VertexBufferCache> getVboCache();
	shared_ptr<TextureCache> getTextureCache();
	shared_ptr<Scene> getScene();
	shared_ptr<RenderOptions> getRenderOptions();
	shared_ptr<JobManager> getJobManager();
	int getFPS();

	void setJobManager(shared_ptr<JobManager> jobManager);
	void setFPS(int fps);
};