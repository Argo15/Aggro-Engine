#pragma once

#include <memory>
#include "../../Core/Object/Cameras/Camera.hpp"
#include "SceneNode.hpp"
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
	shared_ptr<SceneNode> m_root;
	shared_ptr<Camera> m_camera;

public:
	Scene(shared_ptr<SceneNode> root, shared_ptr<Camera> camera);

	shared_ptr<SceneNode> getRoot();
	shared_ptr<Camera> getCamera();
};