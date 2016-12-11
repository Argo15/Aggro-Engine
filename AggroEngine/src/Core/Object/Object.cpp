#include "Object.hpp"

Object::Object()
	: m_transformComponent(shared_ptr<TransformComponent>(new TransformComponent()))
	, m_renderComponent(shared_ptr<RenderComponent>(nullptr))
{

}

void Object::translate(const glm::vec3 &translate)
{
	m_transformComponent->translate(translate);
}

void Object::rotate(const glm::mat4 &rotate)
{
	m_transformComponent->rotate(rotate);
}

void Object::rotate(float angle, const glm::vec3 &axis)
{
	m_transformComponent->rotate(angle, axis);
}

void Object::scale(const glm::vec3 &scale)
{
	m_transformComponent->scale(scale);
}

glm::mat4 Object::getTransform()
{
	return m_transformComponent->getTransform();
}

bool Object::hasTransformComponent()
{
	return m_transformComponent != nullptr;
}

void Object::setTransformComponent(shared_ptr<TransformComponent> transformComponent)
{
	m_transformComponent = transformComponent;
}

shared_ptr<TransformComponent> Object::getTransformComponent()
{
	return m_transformComponent;
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