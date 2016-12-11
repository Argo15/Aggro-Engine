#pragma once

#include <mutex>
#include <boost/thread/lockable_adapter.hpp>
#include "Matrix.hpp"
#include "Component.hpp"
#include "Listener.hpp"

class TransformComponent : public Component,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	glm::mat4 m_rotateMat;
	glm::mat4 m_translateMat;
	glm::mat4 m_scaleMat;
	glm::vec3 m_translate;
	glm::vec3 m_scale;

	Listener<TransformComponent *> m_changeListeners;

public:
	TransformComponent();

	glm::mat4 getTransform();

	glm::vec3 *getTranslate();
	glm::vec3 *getScale();

	void translate(const glm::vec3 &translate);
	void rotate(const glm::mat4 &rotate);
	void rotate(float angle, const glm::vec3 &axis);
	void scale(const glm::vec3 &scale);

	void setTranslate(glm::vec3 translate);
	void setRotate(glm::mat4 rotate);
	void setScale(glm::vec3 scale);

	void addChangeListener(void *ns, std::function<void(TransformComponent *)> listener);
	void removeChangeListener(void *ns);
};