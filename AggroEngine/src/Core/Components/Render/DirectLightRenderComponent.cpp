#include "DirectLightRenderComponent.hpp"
#include "DirectLightMesh.hpp"
#include "RGBImage.hpp"

const string DirectLightRenderComponent::s_imagePath = "Resources/Textures/Engine/sun_icon.png";
const string DirectLightRenderComponent::s_alphaPath = "Resources/Textures/Engine/sun_alpha.png";

DirectLightRenderComponent::DirectLightRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderData()
{
}

DirectLightRenderComponent::DirectLightRenderComponent(shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderData()
{
}

shared_ptr<Chunk> DirectLightRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes;
	shared_ptr<Chunk> chunk(new Chunk(ChunkType::DIRECT_LIGHT_RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
	bytes = ByteAccumulator();
	bytes.add(chunk.get());
	return shared_ptr<Chunk>(new Chunk(ChunkType::RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<RenderComponent> DirectLightRenderComponent::deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::DIRECT_LIGHT_RENDER_COMPONENT)
	{
		return shared_ptr<DirectLightRenderComponent>();
	}

	return shared_ptr<DirectLightRenderComponent>(new DirectLightRenderComponent(byteChunk, resources));
}

void DirectLightRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, shared_ptr<SceneNode> node)
{
	if (!m_lineRenderData)
	{
		m_lineRenderData = shared_ptr<RenderData>(new RenderData());
		m_lineRenderData->setVertexBufferHandle(context->getGraphics()
			->createVertexBuffer(shared_ptr<Mesh>(new DirectLightMesh(-1))));

		shared_ptr<Material> mat(new Material(glm::vec3(1.0)));
		mat->setTexture(context->getGraphics()->createTexture(
			shared_ptr<Image>(new RGBImage(1, 1, glm::vec3(0.75, 0.75, 0.25)))));
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
	context->getGraphics()->stageTriangleRender(m_lineRenderData);
	SpriteRenderComponent::render(context, m4Transform, m4ViewMat, node);
}
