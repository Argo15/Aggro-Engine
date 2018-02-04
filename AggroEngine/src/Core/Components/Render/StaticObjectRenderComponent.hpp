#pragma once

#include <boost/optional.hpp>
#include "RenderComponent.hpp"
#include "Serialization.hpp"
#include "SceneNode.hpp"

class StaticObjectRenderComponent : public RenderComponent
{
private:
	bool m_lightingEnabled;
	bool m_shadowsEnabled;

	void _renderMesh(shared_ptr<Mesh> mesh, shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat, shared_ptr<SceneNode> node);

	StaticObjectRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources);

public:
	StaticObjectRenderComponent();

	// Serialization
	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat, shared_ptr<SceneNode> node);

	void setLightingEnabled(int lightingEnabled);
	bool getLightingEnabled();

	void setShadowsEnabled(int shadowsEnabled);
	bool getShadowsEnabled();
};