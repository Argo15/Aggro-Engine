#include "EngineContext.hpp"

EngineContext::EngineContext()
	: m_jobManager(new JobManager())
	, m_scene(new Scene())
	, m_renderOptions(new RenderOptions())
	, m_resources(new Resources())
{
	m_graphicsContext = shared_ptr<GraphicsContext>(new GraphicsContext(m_jobManager, m_resources));
}

shared_ptr<GraphicsContext> EngineContext::getGraphicsContext()
{
	return m_graphicsContext;
}

shared_ptr<Graphics> EngineContext::getGraphics()
{
	return m_graphicsContext->getGraphics();
}

shared_ptr<VertexBufferCache> EngineContext::getVboCache()
{
	return m_graphicsContext->getVboCache();
}

shared_ptr<TextureCache> EngineContext::getTextureCache()
{
	return m_graphicsContext->getTextureCache();
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