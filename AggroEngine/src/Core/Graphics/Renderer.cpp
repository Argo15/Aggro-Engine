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
	m_gridRenderData->setId(Scene::getNextId());
	m_graphicsContext->getGraphics()->stageRender(m_gridRenderData);
}

void Renderer::renderScene(shared_ptr<Scene> scene, shared_ptr<RenderOptions> renderOptions)
{
	// set scene options
	renderOptions->clear();
	renderOptions->setCamera(scene->getCamera());
	shared_ptr<PerspectiveFrustrum> overrideFrustrum = scene->getOverrideFrustrum();
	if (overrideFrustrum)
	{
		renderOptions->setFrustrum(overrideFrustrum);
	}
	m_graphicsContext->getGraphics()->clearDepthAndColor(renderOptions->getDefaultFrameBufferId()); // clear
		

	if (scene->getTransformHook())
	{
		scene->getTransformHook()->render(m_graphicsContext->getGraphics(), scene); // Render transformer
	}
	// Convert to render chain
	_renderSceneNodeRecursive(scene->getRoot(), glm::mat4(1.0), scene->getCamera()->getViewMatrix(), renderOptions);


	// execute
	m_graphicsContext->getGraphics()->executeRender(*(renderOptions.get()));
}


void Renderer::_renderSceneNodeRecursive(shared_ptr<SceneNode> node, glm::mat4 &transform, glm::mat4 &viewMat, shared_ptr<RenderOptions> renderOptions)
{
	glm::mat4 curTransform = transform;
	if (node->hasTransformComponent())
	{
		curTransform = transform * node->getTransformComponent()->getTransform();
		if (node->hasRenderComponent())
		{
			node->getRenderComponent()->render(m_graphicsContext, curTransform, viewMat);
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