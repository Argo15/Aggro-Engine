#pragma once

#include <boost/optional.hpp>
#include "RenderComponent.hpp"
#include "Serialization.hpp"

class StaticObjectRenderComponent : public RenderComponent
{
private:
	boost::optional<int> m_meshId;
	boost::optional<int> m_textureImageId;

	StaticObjectRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources);

public:
	StaticObjectRenderComponent();

	// Serialization
	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, int objId, bool lightingEnabled);

	void setMeshId(int meshId);
	boost::optional<int> getMeshId();
	
	void setTextureImageId(int imageId);
	boost::optional<int> getTextureImageId();
};