#include "CameraRenderComponent.hpp"
#include "FrustrumMesh.hpp"
#include "RGBImage.hpp"

const string CameraRenderComponent::s_imagePath = "Resources/Textures/Engine/camera_icon.png";
const string CameraRenderComponent::s_alphaPath = "Resources/Textures/Engine/camera_alpha.png";

CameraRenderComponent::CameraRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderData()
{
}

CameraRenderComponent::CameraRenderComponent(shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderData()
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

void CameraRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat, shared_ptr<SceneNode> node)
{
	if (!node->isSelected())
	{
		SpriteRenderComponent::render(context, glm::scale(m4Transform, glm::vec3(0.3, 0.3, 1)), m4ViewMat, node);
		return;
	}
	if (node->getCameraComponent()->isActive())
	{
		return;
	}
	if (!m_lineRenderData)
	{
		m_lineRenderData = shared_ptr<RenderData>(new RenderData());

		shared_ptr<Material> mat(new Material(glm::vec3(1.0)));
		mat->setTexture(context->getGraphics()->createTexture(
			shared_ptr<ImageUC>(new RGBImage(1, 1, glm::vec3(0, 1, 1)))));
		m_lineRenderData->setMaterial(mat);

		m_lineRenderData->setDrawMode(DrawMode::LINES);
		m_lineRenderData->setLineWidth(2);
		m_lineRenderData->setLightingEnabled(false);
		m_lineRenderData->setShadowsEnabled(false);
		if (node)
		{
			m_lineRenderData->setId(node->getId());
		}
	}
	m_lineRenderData->setModelMatrix(m4Transform);
	shared_ptr<Frustrum> frustrum = node->getCamera()->getFrustrum();
	if (frustrum && !frustrum->isSame(m_curFrustrum))
	{
		m_curFrustrum = frustrum;
		m_lineRenderData->setVertexBufferHandle(context->getGraphics()
			->createVertexBuffer(shared_ptr<Mesh>(new FrustrumMesh(-1, m_curFrustrum))));
	}
	context->getGraphics()->stageRender(m_lineRenderData);
	SpriteRenderComponent::render(context, glm::scale(m4Transform, glm::vec3(0.3, 0.3, 1)), m4ViewMat, node);
}
