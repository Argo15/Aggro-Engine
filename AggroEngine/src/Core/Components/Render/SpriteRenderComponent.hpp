#pragma once

#include <boost/optional.hpp>
#include "StaticObjectRenderComponent.hpp"
#include "Serialization.hpp"

class SpriteRenderComponent : public StaticObjectRenderComponent
{
private:
	SpriteRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources);

public:
	SpriteRenderComponent(shared_ptr<Resources> resources, int imageId);

	// Serialization
	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, int objId);
};