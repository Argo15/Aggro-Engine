#pragma once

#include <memory>
#include <vector>
#include "TransformComponent.hpp"
#include "RenderComponent.hpp"
#include "Listener.hpp"
#include "Chunk.hpp"
using namespace std;

/**
* Node of the scene graph. Transforms apply to children nodes
*
* author: wcrane
* since: 2014-05-18
**/
class SceneNode
{
private:
	SceneNode *m_parent;
	shared_ptr<vector<shared_ptr<SceneNode>>> m_children;
	bool m_isSelected;
	string m_name;
	unsigned int m_id;

	// Components
	shared_ptr<TransformComponent> m_transformComponent;
	shared_ptr<RenderComponent> m_renderComponent;

	Listener<SceneNode *> m_changeListeners;
	Listener<SceneNode *> m_deletedListeners;

	SceneNode(Chunk * const byteChunk, shared_ptr<Resources> resources);

public:
	SceneNode(unsigned int m_id, SceneNode *parent = nullptr);

	// Serialization
	shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<SceneNode> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	shared_ptr<vector<shared_ptr<SceneNode>>> getChildren();
	void setSelected(bool selected);
	bool isSelected();
	string getName();
	unsigned int getId();

	void addChild(shared_ptr<SceneNode> child);
	void setName(string name);
	void setParent(SceneNode *parent);

	SceneNode *getParent();

	void addChangeListener(void *ns, std::function<void(SceneNode *)> listener);
	void removeChangeListener(void *ns);
	void notifyChanged();

	void addDeletedListener(void *ns, std::function<void(SceneNode *)> listener);
	void notifyDeleted();

	void removeChild(shared_ptr<SceneNode> child);

	glm::vec3 getWorldTranslate();
	glm::mat4 getWorldTransform();
	glm::mat4 getOrthogonalWorldTransform();
	glm::mat4 getParentTransform();
	glm::mat4 getOrthogonalParentTransform();
	glm::mat4 getObjectTransform();
	glm::mat4 getOrthogonalObjectTransform();

	//Components
	bool hasTransformComponent();
	void setTransformComponent(shared_ptr<TransformComponent> transformComponent);
	shared_ptr<TransformComponent> getTransformComponent();

	bool hasRenderComponent();
	void setRenderComponent(shared_ptr<RenderComponent> renderComponent);
	shared_ptr<RenderComponent> getRenderComponent();
};