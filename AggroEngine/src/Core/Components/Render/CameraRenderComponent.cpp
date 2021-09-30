#include "CameraRenderComponent.hpp"
#include "FrustrumMesh.hpp"
#include "RGBImage.hpp"

const string CameraRenderComponent::s_imagePath = "Resources/Textures/Engine/camera_icon.png";
const string CameraRenderComponent::s_alphaPath = "Resources/Textures/Engine/camera_alpha.png";

CameraRenderComponent::CameraRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderHandle()
{
}

CameraRenderComponent::CameraRenderComponent(shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderHandle()
{
}

shared_ptr<Chunk> CameraRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes = ByteAccumulator();
	bytes.add(shared_ptr<Chunk>(new Chunk(ChunkType::CAMERA_RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect())));
	return shared_ptr<Chunk>(new Chunk(ChunkType::RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<RenderComponent> CameraRenderComponent::deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::CAMERA_RENDER_COMPONENT)
	{
		return shared_ptr<CameraRenderComponent>();
	}

	return shared_ptr<CameraRenderComponent>(new CameraRenderComponent(byteChunk, resources));
}

void CameraRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat)
{
	if (!m_sceneNode->isSelected())
	{
		SpriteRenderComponent::render(context, glm::scale(m4Transform, glm::vec3(0.3, 0.3, 1)), m4ViewMat);
		if (m_lineRenderHandle) m_lineRenderHandle->unstageRender();
		return;
	}
	if (m_sceneNode->getCameraComponent()->isActive())
	{
		if (m_lineRenderHandle) m_lineRenderHandle->unstageRender();
		return;
	}
	if (!m_lineRenderHandle)
	{
		shared_ptr<RenderData> lineRenderData = shared_ptr<RenderData>(new RenderData());
		lineRenderData->setId(Scene::getNextId());

		shared_ptr<Material> mat(new Material(glm::vec3(1.0)));
		mat->setTexture(context->getGraphics()->createTexture(
			shared_ptr<ImageUC>(new RGBImage(1, 1, glm::vec3(0, 1, 1)))));
		lineRenderData->setMaterial(mat);

		lineRenderData->setDrawMode(DrawMode::LINES);
		lineRenderData->setLineWidth(2);
		lineRenderData->setLightingEnabled(false);
		lineRenderData->setShadowsEnabled(false);
		shared_ptr<Frustrum> frustrum = m_sceneNode->getCamera()->getFrustrum();
		if (frustrum && !frustrum->isSame(m_curFrustrum))
		{
			m_curFrustrum = frustrum;
			lineRenderData->setVertexBufferHandle(context->getGraphics()
				->createVertexBuffer(shared_ptr<Mesh>(new FrustrumMesh(-1, m_curFrustrum))));
		}
		m_lineRenderHandle = context->getGraphics()->stageRender(lineRenderData);
	}
	m_lineRenderHandle->getRenderData()->setModelMatrix(m4Transform);
	m_lineRenderHandle->restageRender();
	SpriteRenderComponent::render(context, glm::scale(m4Transform, glm::vec3(0.3, 0.3, 1)), m4ViewMat);
}

void CameraRenderComponent::onSceneNodeDeleted(SceneNode *node)
{
	if (m_lineRenderHandle) m_lineRenderHandle->unstageRender();
	SpriteRenderComponent::onSceneNodeDeleted(node);
}