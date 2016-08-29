#pragma once

#include <QWidget>
#include <QTreeWidgetItem>
#include "SceneNode.hpp"

class SceneNodeTreeItem : public QTreeWidgetItem
{
private:
	shared_ptr<SceneNode> m_sceneNode;

public:
	SceneNodeTreeItem(shared_ptr<SceneNode> sceneNode, QTreeWidget *parent = nullptr);

	shared_ptr<SceneNode> getSceneNode();
};