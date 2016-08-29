#pragma once

#include "Scene.hpp"
#include "SceneNodeTreeItem.hpp"
#include "EngineContext.hpp"
#include <QDockWidget>
#include <memory>
using namespace std;

/**
* Scene graph viewer.
*
* author: wcrane
* since: 2016-08-28
**/
class SceneGraphTree : public QDockWidget
{
private:
	shared_ptr<QTreeWidget> m_treeWidget;
	shared_ptr<EngineContext> m_context;
	QAction *m_addAction;

	void _addSceneNodeRecursive(shared_ptr<SceneNode> node, QTreeWidgetItem *parent, bool isRoot);
	void _addNewNode();
	void _selectionChanged();

public:
	SceneGraphTree(shared_ptr<EngineContext> context, QWidget *parent);

	void refresh(Scene* scene);
};