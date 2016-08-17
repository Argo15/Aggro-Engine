#pragma once

#include <memory>
#include "../../Core/Object/Object.hpp"
#include "../../Core/Object/Cameras/Camera.hpp"
using namespace std;

/**
 * Hierarchical structure of a scene (aka scene graph)
 *
 * author: wcrane
 * since: 2014-05-18
**/
class Scene
{
private:
	shared_ptr<Object> m_rootObject;
	shared_ptr<Camera> m_camera;

public:
	Scene(shared_ptr<Object> rootObjectTmp, shared_ptr<Camera> camera);

	shared_ptr<Object> getRoot();
	shared_ptr<Camera> getCamera();
};