#include "DirectLightRenderComponent.hpp"
#include "DirectLightMesh.hpp"
#include "RGBImage.hpp"

const string DirectLightRenderComponent::s_imagePath = "Resources/Textures/Engine/sun_icon.png";
const string DirectLightRenderComponent::s_alphaPath = "Resources/Textures/Engine/sun_alpha.png";

DirectLightRenderComponent::DirectLightRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderHandle()
{
}

DirectLightRenderComponent::DirectLightRenderComponent(shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
	, m_lineRenderHandle()
{
}

shared_ptr<Chunk> DirectLightRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes = ByteAccumulator();
	bytes.add(shared_ptr<Chunk>(new Chunk(ChunkType::DIRECT_LIGHT_RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect())));
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

void DirectLightRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat)
{
	if (!m_lineRenderHandle)
	{
		shared_ptr<RenderData> lineRenderData = shared_ptr<RenderData>(new RenderData());
		lineRenderData->setId(Scene::getNextId());
		lineRenderData->setVertexBufferHandle(context->getGraphics()
			->createVertexBuffer(shared_ptr<Mesh>(new DirectLightMesh(-1))));

		shared_ptr<Material> mat(new Material(glm::vec3(1.0)));
		mat->setTexture(context->getGraphics()->createTexture(
			shared_ptr<ImageUC>(new RGBImage(1, 1, glm::vec3(0.75, 0.75, 0.25)))));
		lineRenderData->setMaterial(mat);

		lineRenderData->setDrawMode(DrawMode::LINES);
		lineRenderData->setLineWidth(2);
		lineRenderData->setLightingEnabled(false);
		lineRenderData->setShadowsEnabled(false);
		m_lineRenderHandle = context->getGraphics()->stageRender(lineRenderData);
	}
	else 
	{
		m_lineRenderHandle->getRenderData()->setModelMatrix(m4Transform);
	}
	SpriteRenderComponent::render(context, m4Transform, m4ViewMat);
}

void DirectLightRenderComponent::onSceneNodeDeleted(SceneNode *node)
{
	if (m_lineRenderHandle) m_lineRenderHandle->unstageRender();
	if (m_renderHandle) m_renderHandle->unstageRender();
	SpriteRenderComponent::onSceneNodeDeleted(node);
}