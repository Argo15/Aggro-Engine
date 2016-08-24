#include "Scene.hpp"

Scene::Scene(shared_ptr<SceneNode> root, shared_ptr<Camera> camera)
	: m_root(root)
	, m_camera(camera)
{

}

shared_ptr<SceneNode> Scene::getRoot()
{
	return m_root;
}

shared_ptr<Camera> Scene::getCamera()
{
	return m_camera;
}