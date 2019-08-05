#pragma once

#include <QDockWidget>
#include <memory>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include "Scene.hpp"
#include "SceneNodeTreeItem.hpp"
#include "EngineContext.hpp"
#include "StaticObjectRenderComponent.hpp"
using namespace std;

/**
* Scene graph viewer.
*
* author: wcrane
* since: 2016-08-28
**/
class SceneGraphTree : public QDockWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
	Q_OBJECT

private:
	shared_ptr<QTreeWidget> m_treeWidget;
	shared_ptr<EngineContext> m_context;
	QTreeWidgetItem *m_materialsItem;
	QAction *m_addCubeAction;
	QAction *m_addSphereAction;
	QAction *m_deleteAction;

	boost::unordered_map<SceneNode *, QTreeWidgetItem *> m_currentNodes;

	void _addSceneNodeRecursive(shared_ptr<SceneNode> node, QTreeWidgetItem *parent, bool isRoot);
	void _addNewNode(shared_ptr<SceneNode> node);
	void _selectionChanged();

	void _deleteSelected();

	void _refreshNode(SceneNode *node);
	void _deleteNode(SceneNode *node);
	void _selectNode(SceneNode *node);

	shared_ptr<MaterialComponent> _addMaterial(string name = "New Material");
	shared_ptr<SceneNode> _addMeshNode(string name, string path);
	shared_ptr<SceneNode> _addSpriteNode(string name, string path);
	void _addSceneNodeFromFile(string name, string path);


public:
	SceneGraphTree(shared_ptr<EngineContext> context, QWidget *parent);

	void refresh(Scene *scene);

protected:
	void keyPressEvent(QKeyEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);
};