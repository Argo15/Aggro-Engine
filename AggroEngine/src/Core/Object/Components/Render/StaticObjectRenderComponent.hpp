#pragma once

#include "RenderComponent.hpp"

class StaticObjectRenderComponent : public RenderComponent
{
private:
	shared_ptr<VertexBufferHandle> m_vertexBuffer;
	shared_ptr<TextureHandle> m_texture;

public:
	StaticObjectRenderComponent();

	void render(shared_ptr<Graphics> graphics, glm::mat4 m4Transform);

	void setVertexBuffer(shared_ptr<VertexBufferHandle> vertexBuffer);
	shared_ptr<VertexBufferHandle> getVertexBuffer();
	
	void setTexture(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getTexture();
};