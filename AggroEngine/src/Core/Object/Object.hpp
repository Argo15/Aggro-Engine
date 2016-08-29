#pragma once

#include "Components\Render\RenderComponent.hpp"
#include "../../Util/Math/Matrix.hpp"
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
	glm::mat4 m_m4Transform;
	shared_ptr<RenderComponent> m_renderComponent;
	
public:
	Object();

	virtual void setTransform(glm::mat4 &m4Transform);
	virtual void applyTransform(glm::mat4 &m4Transform);
	glm::mat4 &getTransform();

	void translate(glm::vec3 &translate);

	bool hasRenderComponent();
	void setRenderComponent(shared_ptr<RenderComponent> renderComponent);
	shared_ptr<RenderComponent> getRenderComponent();
};