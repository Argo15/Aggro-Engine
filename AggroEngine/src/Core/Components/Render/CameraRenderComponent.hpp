#pragma once

#include "SpriteRenderComponent.hpp"

/**
* Renders a camera frustrum
*
* author: wcrane
* since: 2018-03-13
**/
class CameraRenderComponent : public SpriteRenderComponent
{
private:
	shared_ptr<RenderHandle> m_lineRenderHandle;
	shared_ptr<Frustrum> m_curFrustrum;

	CameraRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources);

public:
	CameraRenderComponent(shared_ptr<Resources> resources);

	const static string s_imagePath;
	const static string s_alphaPath;

	// Serialization
	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4& m4Transform, glm::mat4 &m4ViewMat);

	virtual void onSceneNodeDeleted(SceneNode *node);
};