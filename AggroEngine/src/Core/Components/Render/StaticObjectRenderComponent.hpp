#pragma once

#include <boost/optional.hpp>
#include "RenderComponent.hpp"

class StaticObjectRenderComponent : public RenderComponent
{
private:
	boost::optional<int> m_meshId;
	boost::optional<int> m_textureImageId;

public:
	StaticObjectRenderComponent();

	void render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, int objId);

	void setMeshId(int meshId);
	boost::optional<int> getMeshId();
	
	void setTextureImageId(int meshId);
	boost::optional<int> getTextureImageId();
};