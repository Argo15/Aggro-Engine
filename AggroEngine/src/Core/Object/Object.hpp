#pragma once

#include "RenderComponent.hpp"
#include "TransformComponent.hpp"
#include "Matrix.hpp"
#include <memory>
using namespace std;

/**
 * Base object class. Holds a transform matrix  
 * representing position, orientation, and scale.
 *
 * author: wcrane
 * since: 2013-12-14
**/
class Object
{
private:
	shared_ptr<TransformComponent> m_transformComponent;
	shared_ptr<RenderComponent> m_renderComponent;
	
public:
	Object();

	glm::mat4 getTransform();

	void translate(glm::vec3 &translate);
	void rotate(glm::mat4 &rotate);
	void rotate(float angle, glm::vec3 &axis);
	void scale(glm::vec3 &scale);

	bool hasTransformComponent();
	void setTransformComponent(shared_ptr<TransformComponent> transformComponent);
	shared_ptr<TransformComponent> getTransformComponent();

	bool hasRenderComponent();
	void setRenderComponent(shared_ptr<RenderComponent> renderComponent);
	shared_ptr<RenderComponent> getRenderComponent();
};