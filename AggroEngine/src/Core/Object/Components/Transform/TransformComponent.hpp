#pragma once

#include "Matrix.hpp"
#include "Component.hpp"
#include "Listener.hpp"

class TransformComponent : public Component
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

	void translate(glm::vec3 &translate);
	void rotate(glm::mat4 &rotate);
	void rotate(float angle, glm::vec3 &axis);
	void scale(glm::vec3 &scale);

	void setTranslate(glm::vec3 translate);
	void setRotate(glm::mat4 rotate);
	void setScale(glm::vec3 scale);

	void addChangeListener(void *ns, std::function<void(TransformComponent *)> listener);
	void removeChangeListener(void *ns);
};