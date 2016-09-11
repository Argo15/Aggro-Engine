#pragma once

#include <memory>
#include <vector>
#include "Object.hpp"
#include "Listener.hpp"
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
	SceneNode *m_parent;
	shared_ptr<Object> m_object;
	shared_ptr<vector<shared_ptr<SceneNode>>> m_children;
	bool m_isSelected;
	string m_name;

	Listener<SceneNode *> m_changeListeners;

public:
	SceneNode(SceneNode *parent = nullptr, shared_ptr<Object> object = shared_ptr<Object>(new Object()));

	shared_ptr<Object> getObject();
	shared_ptr<vector<shared_ptr<SceneNode>>> getChildren();
	void setSelected(bool selected);
	bool isSelected();
	string getName();

	void addChild(shared_ptr<SceneNode> child);
	void setName(string name);
	void setParent(SceneNode *parent);

	void addChangeListener(void *ns, std::function<void(SceneNode *)> listener);
	void removeChangeListener(void *ns);
};