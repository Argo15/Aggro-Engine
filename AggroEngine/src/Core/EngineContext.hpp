#pragma once

#include "OpenGL43Graphics.hpp"
#include "Scene.hpp"
#include "VertexBufferCache.hpp"
#include "TextureCache.hpp"

class EngineContext
{
private:
	shared_ptr<Graphics> m_graphics;
	shared_ptr<VertexBufferCache> m_vboCache;
	shared_ptr<TextureCache> m_textureCache;
	shared_ptr<Scene> m_scene;
	int m_fps;

public:
	EngineContext();

	shared_ptr<Graphics> getGraphics();
	shared_ptr<VertexBufferCache> getVboCache();
	shared_ptr<TextureCache> getTextureCache();
	shared_ptr<Scene> getScene();
	int getFPS();

	void setFPS(int fps);
};