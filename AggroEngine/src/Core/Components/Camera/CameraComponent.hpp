#pragma once

#include "TransformComponent.hpp"
#include "Matrix.hpp"
#include "Frustrum.hpp"
#include "Camera.hpp"
#include <memory>
using namespace std;

class CameraComponent
{
private:
	glm::vec4 m_v4Viewport;
	float m_fov;
	float m_aspectRatio;
	float m_zNear;
	float m_zFar;

	CameraComponent(Chunk * const byteChunk);

public:
	CameraComponent();

	// Serialization
	shared_ptr<Chunk> serialize();
	static shared_ptr<CameraComponent> deserialize(Chunk * const byteChunk);

	void setProjection(float fov, float aspectRatio, float zNear, float zFar);
	void setViewport(glm::vec4 &viewport);

	glm::vec4 &getViewport();

	shared_ptr<Camera> getCamera(shared_ptr<TransformComponent> transformComponent);
};