#include "TransformHook.hpp"
#include "LineMesh.hpp"
#include "RGBImage.hpp"
#include "MatrixDecompose.hpp"
#include "Line.hpp"
#include <Qt>

const glm::vec3 axis[] = { glm::vec3(1.0f, 0, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0, 0, 1.0f) };

TransformHook::TransformHook()
	: m_lastPos()
{
}

void TransformHook::_initialize(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene)
{
	for (int i = 0; i < 3; i++)
	{
		m_renderData[i] = shared_ptr<RenderData>(new RenderData());
		m_renderData[i]->setDrawMode(DrawMode::LINES);
		m_renderData[i]->setVertexBufferHandle(graphics->createVertexBuffer(getMesh(axis[i])));
		m_texture[i] = graphics->createTexture(shared_ptr<ImageUC>(new RGBImage(1, 1, axis[i])));
		shared_ptr<Material> mat(new Material(glm::vec3(1.0)));
		mat->setTexture(m_texture[i]);
		m_renderData[i]->setMaterial(mat);
		m_renderData[i]->setLineWidth(2);
		m_renderData[i]->setDepthTestEnabled(false);
		m_renderData[i]->setId(Scene::getNextId());
		m_renderHandle[i] = graphics->stageRender(m_renderData[i]);
	}
	m_texture[3] = graphics->createTexture(shared_ptr<ImageUC>(new RGBImage(1, 1, glm::vec3(1.0f, 1.0f, 0))));
}

void TransformHook::render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene)
{
	// Load if needed
	if (!m_renderHandle[0])
	{
		_initialize(graphics, scene);
	}

	shared_ptr<SceneNode> selectedNode = scene->getSelectedNode();

	if (selectedNode && selectedNode->hasTransformComponent())
	{
		MatrixDecompose transformMat(_getAxisCenter(selectedNode));
		shared_ptr<Camera> camera = scene->getCamera();
		glm::vec3 lookDir = glm::normalize(transformMat.getTranslate() - camera->getEyePos());
		transformMat.setTranslate(camera->getEyePos() + (12.f * lookDir));

		for (int i = 0; i < 3; i++)
		{
			m_renderData[i]->setModelMatrix(transformMat.getOrthogonalTransform());
			m_renderHandle[i]->restageRender();
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			m_renderHandle[i]->unstageRender();
		}
	}
}

bool TransformHook::updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Selection> selection)
{
	auto tracker = PerfStats::instance().trackTime("TransformHook::updateSelection");
	if (!m_renderData[0])
	{
		return false;
	}

	shared_ptr<SceneNode> selectedNode = context->getScene()->getSelectedNode();
	if (!mouse->getButtonPressed(Qt::MouseButton::LeftButton))
	{
		m_lastPos = shared_ptr<glm::vec3>();
		m_selectedIdx = _getSelectedIndex(selection);
		for (int i = 0; i < 3; i++)
		{
			if (i == m_selectedIdx)
			{
				m_renderData[i]->getMaterial()->setTexture(m_texture[3]);
			}
			else
			{
				m_renderData[i]->getMaterial()->setTexture(m_texture[i]);
			}
		}
	}
	else if (m_selectedIdx >= 0 && selectedNode)
	{
		// Get Camera ray
		const glm::mat4 projMat = context->getRenderOptions()->getProjectionMatrix();
		const glm::mat4 viewMat = context->getRenderOptions()->getViewMatrix();
		const glm::vec4 viewport = context->getScene()->getCamera()->getViewport();
		const glm::vec3 lookNearPos = glm::unProject(glm::vec3(mouse->getPosX(), viewport.w - mouse->getPosY(), 0), viewMat, projMat, viewport);
		const glm::vec3 lookFarPos = glm::unProject(glm::vec3(mouse->getPosX(), viewport.w - mouse->getPosY(), 1.0), viewMat, projMat, viewport);
		const glm::vec3 lookDir = glm::normalize(lookFarPos - lookNearPos);
		const Line lookLine(lookNearPos, lookDir);

		updateTransform(selectedNode, lookLine, axis[m_selectedIdx]);
	}
	else if (!selectedNode)
	{
		for (int i = 0; i < 3; i++)
		{
			m_renderHandle[i]->unstageRender();
		}
	}
	return m_selectedIdx >= 0;
}

/*
* Either selects hook directly under mouse or selects hook with most pixels near mouse.
*/
unsigned int TransformHook::_getSelectedIndex(shared_ptr<Selection> selection)
{
	unsigned int id = selection->getSelectionAsId();
	for (int i = 0; i < 3; i++)
	{
		if (id == m_renderData[i]->getId())
		{
			return i;
		}
	}

	int counts[3] = { 0, 0, 0 };
	unsigned int size = selection->getSize();
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			id = selection->getSelectionAsId(x, y);
			for (int i = 0; i < 3; i++)
			{
				if (id == m_renderData[i]->getId())
				{
					counts[i]++;
				}
			}
		}
	}

	if (counts[0] > 0 && counts[0] > counts[1] && counts[0] > counts[2])
	{
		return 0;
	}
	else if (counts[1] > 0 && counts[1] > counts[2])
	{
		return 1;
	}
	else if (counts[2] > 0)
	{
		return 2;
	}
	return -1;
}

void TransformHook::deselect()
{
	m_selectedIdx = -1;
	if (m_renderHandle[0])
	{
		for (int i = 0; i < 3; i++)
		{
			m_renderHandle[i]->unstageRender();
		}
	}
}

glm::mat4 TransformHook::_getAxisCenter(shared_ptr<SceneNode> selectedNode)
{
	return selectedNode->getWorldTransform(selectedNode->getMeshCenter());
}