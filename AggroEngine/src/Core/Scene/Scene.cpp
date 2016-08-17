#include "Scene.hpp"

Scene::Scene(shared_ptr<Object> rootObjectTmp, shared_ptr<Camera> camera)
	: m_rootObject(rootObjectTmp)
	, m_camera(camera)
{

}

shared_ptr<Object> Scene::getRoot()
{
	return m_rootObject;
}

shared_ptr<Camera> Scene::getCamera()
{
	return m_camera;
}