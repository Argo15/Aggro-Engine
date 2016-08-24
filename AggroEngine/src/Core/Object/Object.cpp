#include "Object.hpp"

Object::Object()
	: m_renderComponent(shared_ptr<RenderComponent>(nullptr))
{

}

void Object::setTransform(glm::mat4 &m4Transform)
{
	this->m_m4Transform = m4Transform;
}

void Object::applyTransform(glm::mat4 &m4Transform)
{
	this->m_m4Transform = m4Transform * this->m_m4Transform;
}

glm::mat4 &Object::getTransform()
{
	return this->m_m4Transform;
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