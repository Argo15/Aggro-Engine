#include "EngineContext.hpp"

EngineContext::EngineContext()
	: m_graphics(new OpenGL43Graphics())
	, m_fps(60)
	, m_vboCache(new VertexBufferCache(m_graphics))
	, m_textureCache(new TextureCache(m_graphics))
	, m_scene(new Scene())
	, m_renderOptions(new RenderOptions())
{
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

int EngineContext::getFPS()
{
	return m_fps;
}

void EngineContext::setFPS(int fps)
{
	m_fps = fps;
}