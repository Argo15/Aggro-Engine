#pragma once

#include "Component.hpp"
#include "GraphicsContext.hpp"
#include "Matrix.hpp"
#include "Serialization.hpp"

/**
 * Abstract Component that defines rendering behavior
 *
 * author: wcrane
 * since: 2014-05-18
**/
class RenderComponent : public Component
{
public:
	RenderComponent();

	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources) = 0;
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, int objId, bool lightingEnabled) = 0;
};