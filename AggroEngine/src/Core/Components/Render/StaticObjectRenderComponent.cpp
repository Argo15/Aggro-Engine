#include "StaticObjectRenderComponent.hpp"

StaticObjectRenderComponent::StaticObjectRenderComponent()
	: RenderComponent()
	, m_meshId()
	, m_textureImageId()
{
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