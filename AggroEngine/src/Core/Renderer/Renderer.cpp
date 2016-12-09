#include "Renderer.hpp"
#include "Grid.hpp"
#include "StaticObjectRenderComponent.hpp"

Renderer::Renderer(shared_ptr<Graphics> graphics)
{
	m_graphics = graphics;
}

void Renderer::init(shared_ptr<VertexBufferCache> vboCache, shared_ptr<TextureCache> textureCache)
{
	// Build grid
	shared_ptr<Mesh> grid = unique_ptr<Mesh>(new Grid(16));
	shared_ptr<VertexBufferHandle> gridVBO = m_graphics->createVertexBuffer(grid);
	shared_ptr<StaticObjectRenderComponent> objectRenderComponent(new StaticObjectRenderComponent());
	objectRenderComponent->setVertexBuffer(vboCache->getVertexBuffer("Resources/Mesh/sphere.obj"));
	objectRenderComponent->setTexture(textureCache->getTexture("Resources/Image/Banana2.png"));
	m_gridRenderData = shared_ptr<RenderData>(new RenderData(gridVBO, objectRenderComponent->getTexture(), DrawMode::LINES));
}

void Renderer::renderScene(shared_ptr<Scene> scene, shared_ptr<RenderOptions> renderOptions)
{
	m_graphics->clearDepthAndColor(); // clear
	m_graphics->stageTriangleRender(m_gridRenderData); // Render grid
	scene->getTransformHook()->render(m_graphics, scene); // Render transformer
	_renderSceneNodeRecursive(scene->getRoot(), glm::mat4(1.0)); // Render scene

	// set scene options
	renderOptions->setProjectionMatrix(scene->getCamera()->getProjMatrix());
	renderOptions->setViewMatrix(scene->getCamera()->getViewMatrix());

	// execute
	m_graphics->executeRender(*(renderOptions.get()));
}


void Renderer::_renderSceneNodeRecursive(shared_ptr<SceneNode> node, glm::mat4 transform)
{
	glm::mat4 curTransform = transform;
	shared_ptr<Object> obj = node->getObject();
	if (obj)
	{
		curTransform = transform * obj->getTransform();
		if (obj->hasRenderComponent())
		{
			obj->getRenderComponent()->render(m_graphics, curTransform, node->getId());
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