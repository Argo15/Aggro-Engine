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
	glm::mat4 m_rotateMat;
	glm::mat4 m_translateMat;
	glm::mat4 m_scaleMat;
	glm::vec3 m_translate;
	glm::vec3 m_scale;

	shared_ptr<RenderComponent> m_renderComponent;
	
public:
	Object();

	glm::mat4 getTransform();

	void translate(glm::vec3 &translate);
	void rotate(glm::mat4 &rotate);
	void rotate(float angle, glm::vec3 &axis);
	void scale(glm::vec3 &scale);

	bool hasRenderComponent();
	void setRenderComponent(shared_ptr<RenderComponent> renderComponent);
	shared_ptr<RenderComponent> getRenderComponent();
};