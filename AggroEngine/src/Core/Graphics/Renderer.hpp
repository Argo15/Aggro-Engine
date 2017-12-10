#pragma once

#include "Scene.hpp"
#include "GraphicsContext.hpp"

/**
* Render's a scene graph.
*
* author: wcrane
* since: 2013-11-03
**/
class Renderer
{
private:
	shared_ptr<GraphicsContext> m_graphicsContext;
	shared_ptr<RenderData> m_gridRenderData;

	void _renderSceneNodeRecursive(shared_ptr<SceneNode> node, glm::mat4 transform, glm::mat4 viewMat, shared_ptr<RenderOptions> renderOptions);

public:
	Renderer(shared_ptr<GraphicsContext> graphicsContext);

	void init();
	void renderScene(shared_ptr<Scene> scene, shared_ptr<RenderOptions> renderOptions);
};