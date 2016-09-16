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

void Renderer::renderScene(shared_ptr<Scene> scene)
{
	// clear
	m_graphics->clearDepthAndColor();

	// Render grid
	m_graphics->stageTriangleRender(m_gridRenderData);

	// Render scene
	_renderSceneNodeRecursive(scene->getRoot(), glm::mat4(1.0));

	// set scene options
	RenderOptions renderOptions;
	renderOptions.setProjectionMatrix(scene->getCamera()->getProjMatrix());
	renderOptions.setViewMatrix(scene->getCamera()->getViewMatrix());

	// execute
	m_graphics->executeRender(renderOptions);
}


void Renderer::_renderSceneNodeRecursive(shared_ptr<SceneNode> node, glm::mat4 transform)
{
	glm::mat4 curTransform = transform;
	shared_ptr<Object> obj = node->getObject();
	if (obj)
	{
		curTransform = transform * obj->getTransform();
		if (node->isSelected())
		{
			curTransform = glm::translate(glm::mat4(1.0), glm::vec3(0,1.0f,0)) * curTransform;
		}
		if (obj->hasRenderComponent())
		{
			obj->getRenderComponent()->render(m_graphics, curTransform);
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