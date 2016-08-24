#pragma once

#include <memory>
#include <vector>
#include "../../Core/Object/Object.hpp"
using namespace std;

/**
* Node of the scene graph. Transforms apply to children nodes
*
* author: wcrane
* since: 2014-05-18
**/
class SceneNode
{
private:
	shared_ptr<Object> m_object; // Holds 
	shared_ptr<vector<shared_ptr<SceneNode>>> m_children;

public:
	SceneNode(shared_ptr<Object> object);

	shared_ptr<Object> getObject();
	shared_ptr<vector<shared_ptr<SceneNode>>> getChildren();

	void addChild(shared_ptr<SceneNode> child);
};