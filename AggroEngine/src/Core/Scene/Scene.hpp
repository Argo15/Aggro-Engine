#pragma once

#include <memory>
#include <vector>
#include "Camera.hpp"
#include "SceneNode.hpp"
#include "Listener.hpp"
#include "TransformHook.hpp"
#include "MeshCache.hpp"
#include "Selection.hpp"
using namespace std;

class TransformHook;

/**
 * Hierarchical structure of a scene (aka scene graph)
 *
 * author: wcrane
 * since: 2014-05-18
**/
class Scene
{
private:
	shared_ptr<SceneNode> m_root;
	shared_ptr<SceneNode> m_camera;
	shared_ptr<TransformHook> m_transformHook;
	boost::unordered_map<int, shared_ptr<SceneNode>> m_baseMaterials;
	Listener<Scene*> m_updateListeners;
	Listener<shared_ptr<SceneNode>> m_selectionChangeListeners;
	Listener<shared_ptr<SceneNode>> m_cameraChangeListeners;

	shared_ptr<SceneNode> m_selectedNode;
	shared_ptr<SceneNode> m_previewNode;

	static int s_nextId;

	void _applyToNodeRecursive(shared_ptr<SceneNode> node, std::function<void(SceneNode*)> func);
	shared_ptr<SceneNode> _getNodeByIdRecursive(shared_ptr<SceneNode> node, unsigned int id);
	int _getMaxNodeIdRecursive(shared_ptr<SceneNode> node);

	Scene(Chunk * const byteChunk, shared_ptr<EngineContext> context);

public:
	Scene();
	Scene(shared_ptr<SceneNode> root);
	~Scene();

	// Serialization
	shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<Scene> deserialize(Chunk * const byteChunk, shared_ptr<EngineContext> context);

	shared_ptr<SceneNode> getRoot();

	shared_ptr<SceneNode> getCameraNode();
	shared_ptr<Camera> getCamera();

	void setTransformHook(shared_ptr<TransformHook> transformHook);
	shared_ptr<TransformHook> getTransformHook();

	void setRoot(shared_ptr<SceneNode> root);
	void setCamera(shared_ptr<SceneNode> camera);

	void addUpdateListener(std::function<void(Scene*)> listener);
	void update();

	void applyToAllNodes(std::function<void(SceneNode*)> func);

	void selectNode(shared_ptr<SceneNode> node);
	void selectNodeById(unsigned int id);
	void deselectNode(shared_ptr<SceneNode> node);
	void deselectAllNodes();

	shared_ptr<SceneNode> getSelectedNode();
	shared_ptr<SceneNode> getNodeById(unsigned int id);

	void addSelectionChangeListener(std::function<void(shared_ptr<SceneNode>)> listener);
	void addCameraChangeListener(std::function<void(shared_ptr<SceneNode>)> listener);

	void deleteNode(shared_ptr<SceneNode> node);
	void deleteSelectedNode();

	static unsigned int getNextId();

	void addBaseMaterial(shared_ptr<SceneNode> node);
	boost::unordered_map<int, shared_ptr<SceneNode>> &getBaseMaterials();

	void setPreviewNode(shared_ptr<SceneNode> node);
	shared_ptr<SceneNode> getPreviewNode();

	void update(shared_ptr<Selection> selection, shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Graphics> graphics);

	shared_ptr<PerspectiveFrustrum> getOverrideFrustrum();
};