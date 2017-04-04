#include "TranslateHook.hpp"
#include "LineMesh.hpp"
#include "RGBImage.hpp"
#include "MatrixDecompose.hpp"
#include "Line.hpp"
#include <Qt>

const glm::vec3 transAxis[] = { glm::vec3(1.0f, 0, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0, 0, 1.0f) };

LineHook::LineHook()
	: m_lastPos()
	, m_hasSelection(false)
{

}

void LineHook::_initialize(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene)
{
	for (int i = 0; i < 3; i++)
	{
		m_renderData[i] = shared_ptr<RenderData>(new RenderData());
		m_renderData[i]->setDrawMode(DrawMode::LINES);
		m_renderData[i]->setVertexBufferHandle(graphics->createVertexBuffer(shared_ptr<Mesh>(new LineMesh(glm::vec3(0), 2.0f * transAxis[i]))));
		m_texture[i] = graphics->createTexture(shared_ptr<Image>(new RGBImage(1, 1, transAxis[i])));
		m_renderData[i]->setTextureHandle(m_texture[i]);
		m_renderData[i]->setLineWidth(2);
		m_renderData[i]->setDepthTestEnabled(false);
		m_renderData[i]->setId(scene->getNextId());
		graphics->stageTriangleRender(m_renderData[i]);
	}
	m_texture[3] = graphics->createTexture(shared_ptr<Image>(new RGBImage(1, 1, glm::vec3(1.0f, 1.0f, 0))));
}

void LineHook::render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene)
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
		shared_ptr<Camera> camera = scene->getCamera();
		glm::vec3 lookDir = glm::normalize(transformMat.getTranslate() - camera->getEyePos());
		transformMat.setTranslate(camera->getEyePos() + (12.f * lookDir));

		// Draw
		for (int i = 0; i < 3; i++)
		{
			m_renderData[i]->setModelMatrix(transformMat.getOrthogonalTransform());
			graphics->stageTriangleRender(m_renderData[i]);
		}
	}
}

bool LineHook::updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Selection> selection)
{
	if (!m_renderData[0])
	{
		return false;
	}

	shared_ptr<SceneNode> selectedNode = context->getScene()->getSelectedNode();
	unsigned int id = selection->getSelectionAsId();
	if (!mouse->getButtonPressed(Qt::MouseButton::LeftButton))
	{
		m_hasSelection = false;
		int selectedIndex = _getSelectedIndex(selection);
		for (int i = 0; i < 3; i++)
		{
			if (i == selectedIndex)
			{
				m_renderData[i]->setTextureHandle(m_texture[3]);
				m_hasSelection = true;
				m_selectedIdx = i;
			}
			else
			{
				m_renderData[i]->setTextureHandle(m_texture[i]);
			}
		}
		if (m_lastPos && !m_hasSelection)
		{
			m_lastPos = shared_ptr<glm::vec3>();
		}
	}
	else if (m_hasSelection && selectedNode)
	{
		// Get Camera ray
		const glm::mat4 projMat = context->getRenderOptions()->getProjectionMatrix();
		const glm::mat4 viewMat = context->getRenderOptions()->getViewMatrix();
		const glm::vec4 viewport = context->getGraphics()->getViewport()->asVec4();
		const glm::vec3 lookNearPos = glm::unProject(glm::vec3(mouse->getPosX(), viewport.w - mouse->getPosY(), 0), viewMat, projMat, viewport);
		const glm::vec3 lookFarPos = glm::unProject(glm::vec3(mouse->getPosX(), viewport.w - mouse->getPosY(), 1.0), viewMat, projMat, viewport);
		const glm::vec3 lookDir = glm::normalize(lookFarPos - lookNearPos);
		const Line lookLine(lookNearPos, lookDir);

		// Get selected line
		MatrixDecompose worldMat(selectedNode->getWorldTransform());
		MatrixDecompose parentMat(selectedNode->getParentTransform());
		const glm::vec3 worldScale = worldMat.getScale();
		const glm::vec3 selectionDir = worldMat.getRotate() * transAxis[m_selectedIdx];
		const Line selectionLine(worldMat.getTranslate(), selectionDir);

		const glm::vec3 newPos = lookLine.nearestPoint(selectionLine);
		const glm::mat4 objSpaceTransform = glm::inverse(selectedNode->getParentTransform());
		const glm::vec3 newPosObjSpace = objSpaceTransform * glm::vec4(newPos, 1.0);
		if (m_lastPos)
		{
			glm::vec3 diff = newPosObjSpace - *m_lastPos;
			if (glm::length(diff) > 0)
			{
				updateTransform(selectedNode->getTransformComponent(), diff);
			}
		}
		m_lastPos = shared_ptr<glm::vec3>(new glm::vec3(newPosObjSpace));
	}
	return m_hasSelection;
}

/*
* Either selects hook directly under mouse or selects hook with most pixels near mouse.
*/
unsigned int LineHook::_getSelectedIndex(shared_ptr<Selection> selection)
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