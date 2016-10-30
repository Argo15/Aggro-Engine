#include "EngineContext.hpp"

EngineContext::EngineContext()
	: m_graphics(new OpenGL43Graphics())
	, m_jobManager(new JobManager())
	, m_scene(new Scene())
	, m_renderOptions(new RenderOptions())
{
	m_textureCache = shared_ptr<TextureCache>(new TextureCache(m_graphics, m_jobManager));
	m_vboCache = shared_ptr<VertexBufferCache>(new VertexBufferCache(m_graphics, m_jobManager));
}

shared_ptr<Graphics> EngineContext::getGraphics()
{
	return m_graphics;
}

shared_ptr<VertexBufferCache> EngineContext::getVboCache()
{
	return m_vboCache;
}

shared_ptr<TextureCache> EngineContext::getTextureCache()
{
	return m_textureCache;
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