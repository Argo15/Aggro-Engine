#include "StaticObjectRenderComponent.hpp"

StaticObjectRenderComponent::StaticObjectRenderComponent()
	: RenderComponent()
	, m_meshId()
	, m_textureImageId()
{
}

StaticObjectRenderComponent::StaticObjectRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: RenderComponent()
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	string meshStr = parser.parseString().get_value_or("");
	if (meshStr != "")
	{
		m_meshId = resources->getIdForPath(meshStr);
	}
	string textureStr = parser.parseString().get_value_or("");
	if (textureStr != "")
	{
		m_textureImageId = resources->getIdForPath(textureStr);
	}
}

shared_ptr<Chunk> StaticObjectRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes;
	string meshStr = resources->getPathForId(m_meshId.get_value_or(-1)).get_value_or("");
	bytes.add(&meshStr);
	string textureStr = resources->getPathForId(m_textureImageId.get_value_or(-1)).get_value_or("");
	bytes.add(&textureStr);
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

void StaticObjectRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, int objId)
{
	if (!m_meshId || !m_textureImageId)
	{
		return;
	}

	shared_ptr<VertexBufferHandle> vbo = context->getVboCache()->getVertexBuffer(*m_meshId);
	shared_ptr<TextureHandle> texture = context->getTextureCache()->getTexture(*m_textureImageId);
	if (vbo->isLoaded())
	{
		shared_ptr<RenderData> renderData(new RenderData(vbo, texture));
		renderData->setModelMatrix(m4Transform);
		renderData->setId(objId);
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

void StaticObjectRenderComponent::setTextureImageId(int imageId)
{
	m_textureImageId = imageId;
}

boost::optional<int> StaticObjectRenderComponent::getTextureImageId()
{
	return m_textureImageId;
}