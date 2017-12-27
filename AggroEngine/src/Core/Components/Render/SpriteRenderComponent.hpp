#pragma once

#include <boost/optional.hpp>
#include "StaticObjectRenderComponent.hpp"
#include "Serialization.hpp"

/**
* Renders an image that is always orthoganal to the camera
*
* author: wcrane
* since: 2017-12-08
**/
class SpriteRenderComponent : public StaticObjectRenderComponent
{
private:
	SpriteRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources);

protected:
	SpriteRenderComponent();

public:
	SpriteRenderComponent(shared_ptr<Resources> resources);

	// Serialization
	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, shared_ptr<SceneNode> node);
};