#include "Renderer.hpp"
#include "Grid.hpp"
#include "StaticObjectRenderComponent.hpp"

Renderer::Renderer(shared_ptr<GraphicsContext> graphicsContext)
{
	m_graphicsContext = graphicsContext;
}

void Renderer::init()
{
	// Build grid
	shared_ptr<Mesh> grid = unique_ptr<Mesh>(new Grid(16));
	shared_ptr<VertexBufferHandle> gridVBO = m_graphicsContext->getGraphics()->createVertexBuffer(grid);
	shared_ptr<TextureHandle> defaultTexture = m_graphicsContext->getGraphics()->createTexture();
	m_gridRenderData = shared_ptr<RenderData>(new RenderData(gridVBO, defaultTexture, DrawMode::LINES));
}

void Renderer::renderScene(shared_ptr<Scene> scene, shared_ptr<RenderOptions> renderOptions)
{
	m_graphicsContext->getGraphics()->clearDepthAndColor(); // clear
	m_graphicsContext->getGraphics()->stageTriangleRender(m_gridRenderData); // Render grid
	if (scene->getTransformHook())
	{
		scene->getTransformHook()->render(m_graphicsContext->getGraphics(), scene); // Render transformer
	}
	_renderSceneNodeRecursive(scene->getRoot(), glm::mat4(1.0)); // Render scene

	// set scene options
	renderOptions->setProjectionMatrix(scene->getCamera()->getProjMatrix());
	renderOptions->setViewMatrix(scene->getCamera()->getViewMatrix());

	// execute
	m_graphicsContext->getGraphics()->executeRender(*(renderOptions.get()));
}


void Renderer::_renderSceneNodeRecursive(shared_ptr<SceneNode> node, glm::mat4 transform)
{
	glm::mat4 curTransform = transform;
	if (node->hasTransformComponent())
	{
		curTransform = transform * node->getTransformComponent()->getTransform();
		if (node->hasRenderComponent())
		{
			node->getRenderComponent()->render(m_graphicsContext, curTransform, node->getId());
		}
	}
	shared_ptr<vector<shared_ptr<SceneNode>>> children = node->getChildren();
	if (children->size() > 0)
	{
		for (auto &child : *children.get())
		{
			_renderSceneNodeRecursive(child, curTransform);
		}
	}
}