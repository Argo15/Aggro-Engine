#pragma once

#include "OpenGL43Graphics.hpp"
#include "TextureCache.hpp"
#include "VertexBufferCache.hpp"

class GraphicsContext
{
private:
	shared_ptr<Graphics> m_graphics;
	shared_ptr<VertexBufferCache> m_vboCache;
	shared_ptr<TextureCache> m_textureCache;

public:
	GraphicsContext(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources);

	shared_ptr<Graphics> getGraphics();
	shared_ptr<VertexBufferCache> getVboCache();
	shared_ptr<TextureCache> getTextureCache();
};