#pragma once

#include "SpriteRenderComponent.hpp"

/**
* Renders a sprite with lines indicating the light direction
*
* author: wcrane
* since: 2017-12-08
**/
class DirectLightRenderComponent : public SpriteRenderComponent
{
private:
	shared_ptr<RenderData> m_lineRenderData;

	DirectLightRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources);

public:
	DirectLightRenderComponent(shared_ptr<Resources> resources);

	const static string s_imagePath;

	// Serialization
	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, shared_ptr<SceneNode> node);
};
