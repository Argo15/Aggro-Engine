#include "SceneNodeTreeItem.hpp"

SceneNodeTreeItem::SceneNodeTreeItem(shared_ptr<SceneNode> sceneNode, QTreeWidget *parent)
	: QTreeWidgetItem(parent)
	, m_sceneNode(sceneNode)
{

}

shared_ptr<SceneNode> SceneNodeTreeItem::getSceneNode()
{
	return m_sceneNode;
}