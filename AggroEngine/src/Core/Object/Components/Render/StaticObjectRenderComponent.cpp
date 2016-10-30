#include "StaticObjectRenderComponent.hpp"

StaticObjectRenderComponent::StaticObjectRenderComponent()
	: RenderComponent()
	, m_vertexBuffer(shared_ptr<VertexBufferHandle>(nullptr))
	, m_texture(shared_ptr<TextureHandle>(nullptr))
{
}

void StaticObjectRenderComponent::render(shared_ptr<Graphics> graphics, glm::mat4 m4Transform, int objId)
{
	if (m_vertexBuffer && m_texture && this->getVertexBuffer()->isLoaded())
	{
		shared_ptr<RenderData> renderData = shared_ptr<RenderData>(new RenderData(this->getVertexBuffer(), this->getTexture()));
		renderData->setModelMatrix(m4Transform);
		renderData->setId(objId);
		graphics->stageTriangleRender(renderData);
	}
}

void StaticObjectRenderComponent::setVertexBuffer(shared_ptr<VertexBufferHandle> vertexBuffer)
{
	m_vertexBuffer = vertexBuffer;
}
	
shared_ptr<VertexBufferHandle> StaticObjectRenderComponent::getVertexBuffer()
{
	return m_vertexBuffer;
}
	
void StaticObjectRenderComponent::setTexture(shared_ptr<TextureHandle> texture)
{
	m_texture = texture;
}
	
shared_ptr<TextureHandle> StaticObjectRenderComponent::getTexture()
{
	return m_texture;
}