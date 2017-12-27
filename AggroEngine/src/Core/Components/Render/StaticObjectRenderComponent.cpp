#include "StaticObjectRenderComponent.hpp"

StaticObjectRenderComponent::StaticObjectRenderComponent()
	: RenderComponent()
	, m_meshId()
	, m_lightingEnabled(true)
{
}

StaticObjectRenderComponent::StaticObjectRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: RenderComponent()
	, m_lightingEnabled(true)
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	string meshStr = parser.parseString().get_value_or("");
	if (meshStr != "")
	{
		m_meshId = resources->getIdForPath(meshStr);
	}
}

shared_ptr<Chunk> StaticObjectRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes;
	string meshStr = resources->getPathForId(m_meshId.get_value_or(-1)).get_value_or("");
	bytes.add(&meshStr);
	shared_ptr<Chunk> chunk(new Chunk(ChunkType::STATIC_OBJECT_RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
	bytes = ByteAccumulator();
	bytes.add(chunk.get());
	return shared_ptr<Chunk>(new Chunk(ChunkType::RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<RenderComponent> StaticObjectRenderComponent::deserialize(
	Chunk * const byteChunk, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::STATIC_OBJECT_RENDER_COMPONENT)
	{
		return shared_ptr<StaticObjectRenderComponent>();
	}

	return shared_ptr<StaticObjectRenderComponent>(new StaticObjectRenderComponent(byteChunk, resources));
}

void StaticObjectRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, shared_ptr<SceneNode> node)
{
	if (!m_meshId)
	{
		return;
	}

	shared_ptr<VertexBufferHandle> vbo = context->getVboCache()->getVertexBuffer(*m_meshId);
	if (vbo->isLoaded())
	{
		shared_ptr<RenderData> renderData(new RenderData(vbo));
		renderData->setModelMatrix(m4Transform);
		renderData->setLightingEnabled(m_lightingEnabled);
		if (node)
		{
			renderData->setId(node->getId());
			if (node->hasMaterialComponent())
			{
				renderData->setMaterial(node->getMaterialComponent()->getMaterial(context->getTextureCache()));
			}
		}
		context->getGraphics()->stageTriangleRender(renderData);
	}
}

void StaticObjectRenderComponent::setMeshId(int meshId)
{
	m_meshId = meshId;
}

boost::optional<int> StaticObjectRenderComponent::getMeshId()
{
	return m_meshId;
}

void StaticObjectRenderComponent::setLightingEnabled(int lightingEnabled)
{
	m_lightingEnabled = lightingEnabled;
}

bool StaticObjectRenderComponent::getLightingEnabled()
{
	return m_lightingEnabled;
}