#include "Object.hpp"

Object::Object()
	: m_renderComponent(shared_ptr<RenderComponent>(nullptr))
	, m_rotateMat(glm::mat4(1.0))
	, m_translateMat(glm::mat4(1.0))
	, m_scaleMat(glm::mat4(1.0))
	, m_translate(glm::vec3(0))
	, m_scale(glm::vec3(1))
{

}

void Object::translate(glm::vec3 &translate)
{
	m_translate += translate;
	m_translateMat = glm::translate(glm::mat4(1.0), m_translate);
}

void Object::rotate(glm::mat4 &rotate)
{
	m_rotateMat = rotate * m_rotateMat;
}

void Object::rotate(float angle, glm::vec3 &axis)
{
	rotate(glm::toMat4(glm::angleAxis(angle, glm::normalize(axis))));
}

void Object::scale(glm::vec3 &scale)
{
	m_scale += scale;
	m_scaleMat = glm::scale(glm::mat4(1.0), m_scale);
}

glm::mat4 Object::getTransform()
{
	return m_scaleMat * m_translateMat * m_rotateMat;
}

bool Object::hasRenderComponent()
{
	return m_renderComponent != nullptr;
}

void Object::setRenderComponent(shared_ptr<RenderComponent> renderComponent)
{
	m_renderComponent = renderComponent;
}
	
shared_ptr<RenderComponent> Object::getRenderComponent()
{
	return m_renderComponent;
}