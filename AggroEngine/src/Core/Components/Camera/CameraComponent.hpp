#pragma once

#include "TransformComponent.hpp"
#include "Matrix.hpp"
#include "Frustrum.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include <memory>
using namespace std;

class Scene;

class CameraComponent : public Component
	, public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	glm::vec4 m_v4Viewport;
	float m_fov;
	float m_aspectRatio;
	float m_zNear;
	float m_zFar;
	bool m_isActive;
	Scene *m_scene;
	bool m_overrideFrustrum;
	Listener<CameraComponent *> m_changeListeners;

	CameraComponent(Chunk * const byteChunk, Scene *scene);

public:
	CameraComponent(Scene *scene);
	CameraComponent(shared_ptr<CameraComponent> copy, Scene *scene);

	// Serialization
	shared_ptr<Chunk> serialize();
	static shared_ptr<CameraComponent> deserialize(Chunk * const byteChunk, Scene *scene);

	void setProjection(float fov, float aspectRatio, float zNear, float zFar);
	void setViewport(glm::vec4 &viewport);
	glm::vec4 &getViewport();
	bool isActive();

	void addChangeListener(void *ns, std::function<void(CameraComponent *)> listener);
	void removeChangeListener(void *ns);
	void notify();

	shared_ptr<Camera> getCamera(shared_ptr<TransformComponent> transformComponent);

	void setOverrideFrustrum(bool enabled);
	bool shouldOverrideFrustrum();
};