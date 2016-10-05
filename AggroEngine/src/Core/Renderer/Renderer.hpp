#pragma once

#include "Scene.hpp"
#include "VertexBufferCache.hpp"
#include "TextureCache.hpp"

/**
* Render's a scene graph.
*
* author: wcrane
* since: 2013-11-03
**/
class Renderer
{
private:
	shared_ptr<Graphics> m_graphics;
	shared_ptr<RenderData> m_gridRenderData;

	void _renderSceneNodeRecursive(shared_ptr<SceneNode> node, glm::mat4 transform);

public:
	Renderer(shared_ptr<Graphics> graphics);

	void init(shared_ptr<VertexBufferCache> vboCache, shared_ptr<TextureCache> textureCache);
	void renderScene(shared_ptr<Scene> scene, shared_ptr<RenderOptions> renderOptions);
};