#include "GraphicsContext.hpp"

GraphicsContext::GraphicsContext(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources)
	: m_graphics(new OpenGL43Graphics())
{
	m_vboCache = shared_ptr<VertexBufferCache>(new VertexBufferCache(m_graphics, jobManager, resources));
	m_textureCache = shared_ptr<TextureCache>(new TextureCache(m_graphics, jobManager, resources));
}


shared_ptr<Graphics> GraphicsContext::getGraphics()
{
	return m_graphics;
}

shared_ptr<VertexBufferCache> GraphicsContext::getVboCache()
{
	return m_vboCache;
}

shared_ptr<TextureCache> GraphicsContext::getTextureCache()
{
	return m_textureCache;
}