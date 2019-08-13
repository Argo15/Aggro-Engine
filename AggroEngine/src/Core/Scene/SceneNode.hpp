#pragma once

#include <memory>
#include <vector>
#include "TransformComponent.hpp"
#include "RenderComponent.hpp"
#include "DirectLightComponent.hpp"
#include "MaterialComponent.hpp"
#include "MeshComponent.hpp"
#include "Listener.hpp"
#include "Chunk.hpp"
#include "EngineContext.hpp"
#include "CameraComponent.hpp"
using namespace std;

class RenderComponent;
class MaterialComponent;
class EngineContext;
class CameraComponent;

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
	bool m_isBaseMaterialNode;
	string m_filename;

	// Components
	shared_ptr<TransformComponent> m_transformComponent;
	shared_ptr<RenderComponent> m_renderComponent;
	shared_ptr<DirectLightComponent> m_directLightComponent;
	shared_ptr<MaterialComponent> m_materialComponent;
	shared_ptr<MeshComponent> m_meshComponent;
	shared_ptr<CameraComponent> m_cameraComponent;

	Listener<SceneNode *> m_changeListeners;
	Listener<SceneNode *> m_deletedListeners;

	SceneNode(Chunk * const byteChunk, shared_ptr<EngineContext> context, boost::unordered_map<int, shared_ptr<SceneNode>> baseMaterials, Scene *scene);

public:
	SceneNode(unsigned int m_id, SceneNode *parent = nullptr);

	// Serialization
	shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<SceneNode> deserialize(Chunk * const byteChunk, shared_ptr<EngineContext> context, boost::unordered_map<int, shared_ptr<SceneNode>> baseMaterials, Scene *scene);

	// Some data, such as mesh data, is loaded from a file, to allow for moving in and out of memory.
	// This method will take the a scene node from a file and merge it with the scene node.
	void resolveFileBackedData(shared_ptr<SceneNode> fileBackedSceneNode);

	shared_ptr<vector<shared_ptr<SceneNode>>> getChildren();
	void setSelected(bool selected);
	bool isSelected();
	string getName();
	unsigned int getId();
	string getFilename();

	void addChild(shared_ptr<SceneNode> child);
	void setName(string name);
	void setParent(SceneNode *parent);
	void setFilename(string filename);

	SceneNode *getParent();

	void addChangeListener(void *ns, std::function<void(SceneNode *)> listener);
	void removeChangeListener(void *ns);
	void notifyChanged();

	void addDeletedListener(void *ns, std::function<void(SceneNode *)> listener);
	void notifyDeleted();

	void removeChild(shared_ptr<SceneNode> child);

	glm::vec3 getWorldTranslate();
	glm::mat4 getWorldTransform(glm::vec3 objCenter = glm::vec3(0));
	glm::mat4 getOrthogonalWorldTransform();
	glm::mat4 getParentTransform();
	glm::mat4 getOrthogonalParentTransform();
	glm::mat4 getObjectTransform();
	glm::mat4 getOrthogonalObjectTransform();
	glm::vec3 getMeshCenter();

	//Components
	bool hasTransformComponent();
	void setTransformComponent(shared_ptr<TransformComponent> transformComponent);
	shared_ptr<TransformComponent> getTransformComponent();

	bool hasRenderComponent();
	void setRenderComponent(shared_ptr<RenderComponent> renderComponent);
	shared_ptr<RenderComponent> getRenderComponent();

	bool hasDirectLightComponent();
	void setDirectLightComponent(shared_ptr<DirectLightComponent> directLightComponent);
	shared_ptr<DirectLightComponent> getDirectLightComponent();

	bool hasMaterialComponent();
	void setMaterialComponent(shared_ptr<MaterialComponent> materialComponent);
	shared_ptr<MaterialComponent> getMaterialComponent();
	void setBaseMaterialNode(bool value = true);
	bool isBaseMaterialNode();

	bool hasMeshComponent();
	void setMeshComponent(shared_ptr<MeshComponent> meshComponent);
	shared_ptr<MeshComponent> getMeshComponent();

	bool hasCameraComponent();
	void setCameraComponent(shared_ptr<CameraComponent> cameraComponent);
	shared_ptr<CameraComponent> getCameraComponent();
	shared_ptr<Camera> getCamera();
};