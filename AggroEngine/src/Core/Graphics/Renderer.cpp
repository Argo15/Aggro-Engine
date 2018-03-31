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
	shared_ptr<Mesh> grid = unique_ptr<Mesh>(new Grid(-1, 16));
	shared_ptr<VertexBufferHandle> gridVBO = m_graphicsContext->getGraphics()->createVertexBuffer(grid);
	shared_ptr<TextureHandle> defaultTexture = m_graphicsContext->getGraphics()->createTexture();
	m_gridRenderData = shared_ptr<RenderData>(new RenderData(gridVBO, DrawMode::LINES));
	m_gridRenderData->setLightingEnabled(false);
	m_gridRenderData->setShadowsEnabled(false);
}

void Renderer::renderScene(shared_ptr<Scene> scene, shared_ptr<RenderOptions> renderOptions)
{
	renderOptions->clear();
	m_graphicsContext->getGraphics()->clearDepthAndColor(renderOptions->getDefaultFrameBufferId()); // clear
	m_graphicsContext->getGraphics()->stageRender(m_gridRenderData); // Render grid
	if (scene->getTransformHook())
	{
		scene->getTransformHook()->render(m_graphicsContext->getGraphics(), scene); // Render transformer
	}
	// Render scene
	_renderSceneNodeRecursive(scene->getRoot(), glm::mat4(1.0), scene->getCamera()->getViewMatrix(), renderOptions);
	// Render mesh preview
	_renderSceneNodeRecursive(scene->getPreviewNode(), glm::mat4(1.0), scene->getCamera()->getViewMatrix(), renderOptions);

	// set scene options
	renderOptions->setCamera(scene->getCamera());
	shared_ptr<PerspectiveFrustrum> overrideFrustrum = scene->getOverrideFrustrum();
	if (overrideFrustrum)
	{
		renderOptions->setFrustrum(overrideFrustrum);
	}

	// execute
	m_graphicsContext->getGraphics()->executeRender(*(renderOptions.get()));
}


void Renderer::_renderSceneNodeRecursive(shared_ptr<SceneNode> node, glm::mat4 transform, glm::mat4 viewMat, shared_ptr<RenderOptions> renderOptions)
{
	glm::mat4 curTransform = transform;
	if (node->hasTransformComponent())
	{
		curTransform = transform * node->getTransformComponent()->getTransform();
		if (node->hasRenderComponent())
		{
			node->getRenderComponent()->render(m_graphicsContext, curTransform, viewMat, node);
		}
		if (node->hasDirectLightComponent())
		{
			renderOptions->addDirectLight(node->getDirectLightComponent()->getLight(node->getTransformComponent()));
		}
	}
	shared_ptr<vector<shared_ptr<SceneNode>>> children = node->getChildren();
	if (children->size() > 0)
	{
		for (auto &child : *children.get())
		{
			_renderSceneNodeRecursive(child, curTransform, viewMat, renderOptions);
		}
	}
}