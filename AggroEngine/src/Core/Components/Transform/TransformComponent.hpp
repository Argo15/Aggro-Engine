#pragma once

#include "Locks.hpp"
#include "Matrix.hpp"
#include "Component.hpp"
#include "Listener.hpp"
#include "Serialization.hpp"

class TransformComponent : public Component,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	glm::quat m_rotateMat;
	glm::mat4 m_translateMat;
	glm::mat4 m_scaleMat;
	glm::vec3 m_translate;
	glm::vec3 m_scale;
	glm::mat4 m_fullTransform;

	Listener<TransformComponent *> m_changeListeners;

	void _refresh();

	TransformComponent(Chunk * const byteChunk);

public:
	TransformComponent();
	TransformComponent(shared_ptr<TransformComponent> copy);

	// Serialization
	shared_ptr<Chunk> serialize();
	static shared_ptr<TransformComponent> deserialize(Chunk * const byteChunk);

	glm::mat4 getTransform();

	glm::vec3 getTranslate();
	glm::vec3 getRotateEuler();
	glm::mat4 getRotate();
	glm::vec3 getScale();

	void translate(const glm::vec3 &translate);
	void rotate(float angle, const glm::vec3 &axis, const glm::vec3 &center = glm::vec3(0));
	void scale(const glm::vec3 &scale);

	void setTranslate(glm::vec3 translate);
	void setRotate(glm::quat rotate);
	void setRotate(glm::vec3 rotate);
	void setScale(glm::vec3 scale);

	void addChangeListener(void *ns, std::function<void(TransformComponent *)> listener);
	void removeChangeListener(void *ns);

	void setLookAt(glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up);
};