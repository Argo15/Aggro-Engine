#pragma once

#include "Component.hpp"
#include "GraphicsContext.hpp"
#include "Matrix.hpp"
#include "Serialization.hpp"
#include "SceneNode.hpp"

class SceneNode;

/**
 * Abstract Component that defines rendering behavior
 *
 * author: wcrane
 * since: 2014-05-18
**/
class RenderComponent : public Component
{
protected:
	SceneNode *m_sceneNode; // Not a shared_ptr since m_sceneNode will "own" this component

public:
	RenderComponent();

	virtual shared_ptr<Chunk> serialize(shared_ptr<Resources> resources) = 0;
	static shared_ptr<RenderComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	void setSceneNode(SceneNode *node);
	virtual void onSceneNodeDeleted(SceneNode *node) = 0;

	virtual void render(shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat) = 0;
};