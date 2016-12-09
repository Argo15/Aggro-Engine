#include "TranslateHook.hpp"
#include "LineMesh.hpp"
#include "RGBImage.hpp"
#include "MatrixDecompose.hpp"
#include <Qt>

const glm::vec3 transAxis[] = { glm::vec3(1.0f, 0, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0, 0, 1.0f) };

TranslateHook::TranslateHook()
	: m_hasSelection(false)
{

}

void TranslateHook::_initialize(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene)
{
	for (int i = 0; i < 3; i++)
	{
		m_renderData[i] = shared_ptr<RenderData>(new RenderData());
		m_renderData[i]->setDrawMode(DrawMode::LINES);
		m_renderData[i]->setVertexBufferHandle(graphics->createVertexBuffer(shared_ptr<Mesh>(new LineMesh(glm::vec3(0), 2.0f * transAxis[i]))));
		m_texture[i] = graphics->createTexture(shared_ptr<Image>(new RGBImage(1, 1, transAxis[i])));
		m_renderData[i]->setTextureHandle(m_texture[i]);
		m_renderData[i]->setLineWidth(5);
		m_renderData[i]->setDepthTestEnabled(false);
		m_renderData[i]->setId(scene->getNextId());
		graphics->stageTriangleRender(m_renderData[i]);
	}
	m_texture[3] = graphics->createTexture(shared_ptr<Image>(new RGBImage(1, 1, glm::vec3(1.0f, 1.0f, 0))));
}

void TranslateHook::render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene)
{
	// Load if needed
	if (!m_renderData[0])
	{
		_initialize(graphics, scene);
	}

	shared_ptr<SceneNode> selectedNode = scene->getSelectedNode();

	if (selectedNode)
	{
		MatrixDecompose transformMat(selectedNode->getWorldTransform());

		// Draw
		for (int i = 0; i < 3; i++)
		{
			m_renderData[i]->setModelMatrix(transformMat.getOrthogonalTransform());
			graphics->stageTriangleRender(m_renderData[i]);
		}
	}
}

bool TranslateHook::updateSelection(shared_ptr<MouseState> mouse, shared_ptr<Selection> selection)
{
	if (!m_renderData[0])
	{
		return false;
	}

	unsigned int id = selection->getSelectionAsId();
	if (!mouse->getButtonPressed(Qt::MouseButton::LeftButton))
	{
		m_hasSelection = false;
		for (int i = 0; i < 3; i++)
		{
			if (id == m_renderData[i]->getId())
			{
				m_renderData[i]->setTextureHandle(m_texture[3]);
				m_hasSelection = true;
			}
			else
			{
				m_renderData[i]->setTextureHandle(m_texture[i]);
			}
		}
	}
	return m_hasSelection;
}