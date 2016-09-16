#include "SceneGraphTree.hpp"
#include "SceneTreeWidget.hpp"
#include "StaticObjectRenderComponent.hpp"
#include <QPushButton>
#include <QAction>
#include <QMouseEvent>
#include <QModelIndex>
#include <iostream>
#include <boost/unordered_map.hpp>

SceneGraphTree::SceneGraphTree(shared_ptr<EngineContext> context, QWidget *parent)
	: QDockWidget(parent)
	, m_treeWidget(shared_ptr<QTreeWidget>(new SceneTreeWidget()))
	, m_context(context)
	, m_isRefreshing(false)
{
	m_addCubeAction = new QAction(tr("Add Cube"), this);
	connect(m_addCubeAction, &QAction::triggered, this, [this]() {
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		renderComponent->setVertexBuffer(m_context->getVboCache()->getVertexBuffer("Resources/Mesh/cube.obj"));
		renderComponent->setTexture(m_context->getTextureCache()->getTexture("Resources/Textures/Walls/wall01/wall01_Diffuse.tga"));
		_addNewNode(renderComponent, "Cube");
	});

	m_addSphereAction = new QAction(tr("Add Sphere"), this);
	connect(m_addSphereAction, &QAction::triggered, this, [this]() {
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		renderComponent->setVertexBuffer(m_context->getVboCache()->getVertexBuffer("Resources/Mesh/sphere.obj"));
		renderComponent->setTexture(m_context->getTextureCache()->getTexture("Resources/Textures/Walls/wall01/wall01_Diffuse.tga"));
		_addNewNode(renderComponent, "Sphere");
	});

	m_deleteAction = new QAction(tr("Delete"), this);
	m_deleteAction->setEnabled(false);

	// Create new tree widget
	m_treeWidget = shared_ptr<QTreeWidget>(new SceneTreeWidget(this));
	m_treeWidget->setHeaderHidden(true);
	m_treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	m_treeWidget->addAction(m_addCubeAction);
	m_treeWidget->addAction(m_addSphereAction);
	m_treeWidget->addAction(m_deleteAction);
	connect(m_treeWidget.get(), &QTreeWidget::itemSelectionChanged, this, &SceneGraphTree::_selectionChanged);
	
	m_context->getScene()->addUpdateListener([this](auto scene) {refresh(scene);});
	setMouseTracking(true);
	setWidget(m_treeWidget.get());
}

void SceneGraphTree::refresh(Scene* scene)
{
	m_isRefreshing = true;
	_addSceneNodeRecursive(scene->getRoot(), nullptr, true);
	m_isRefreshing = false;
}

void SceneGraphTree::_addSceneNodeRecursive(shared_ptr<SceneNode> node, QTreeWidgetItem *parent, bool isRoot)
{
	if (!node)
	{
		return;
	}

	QTreeWidgetItem *treeItem = nullptr;
	if (m_currentNodes[node.get()] != nullptr)
	{
		// already in graph, update it
		treeItem = m_currentNodes[node.get()];
		treeItem->setText(0, QString::fromStdString(node->getName()));
	}
	else
	{
		// Create item for current
		if (isRoot)
		{
			setWindowTitle("Scene"); // TODO change to scene name
		}
		else if (parent == nullptr)
		{
			treeItem = new SceneNodeTreeItem(node, m_treeWidget.get());
			treeItem->setText(0, QString::fromStdString(node->getName()));
		}
		else
		{
			treeItem = new SceneNodeTreeItem(node);
			treeItem->setText(0, QString::fromStdString(node->getName()));
			parent->addChild(treeItem);
		}
		m_currentNodes[node.get()] = treeItem;
		node->addChangeListener(this, [this](auto updateNode) {this->_refreshNode(updateNode);});
	}

	if (treeItem != nullptr)
	{
		if (node->isSelected())
		{
			treeItem->setSelected(true);
			if (treeItem->parent() != nullptr)
			{
				treeItem->parent()->setExpanded(true);
			}
		}
		else
		{
			treeItem->setSelected(false);
		}
	}

	// Recursive iterate all children
	shared_ptr<vector<shared_ptr<SceneNode>>> children = node->getChildren();
	if (children->size() > 0)
	{
		for (auto &child : *children.get())
		{
			_addSceneNodeRecursive(child, treeItem, false);
		}
	}
}

void SceneGraphTree::_addNewNode(shared_ptr<StaticObjectRenderComponent> renderComponent, string name)
{
	QTreeWidgetItemIterator it(m_treeWidget.get(), QTreeWidgetItemIterator::Selected);
	shared_ptr<SceneNode> node = m_context->getScene()->getRoot();

	if (*it)
	{
		SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
		node = item->getSceneNode();
	}

	//Create new scene node
	shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(node.get()));
	newNode->getObject()->setRenderComponent(renderComponent);
	newNode->getObject()->translate(glm::vec3(0, 2.0f, 0));
	newNode->getObject()->rotate(45.0f, glm::vec3(1.f, 1.0f, 1.f));
	newNode->setName(name);
	node->addChild(newNode);

	// Update scene graph tree
	m_context->getScene()->deselectAllNodes();
	m_context->getScene()->selectNode(newNode);
	m_context->getScene()->update();
}

void SceneGraphTree::_selectionChanged()
{
	if (!m_isRefreshing)
	{
		m_context->getScene()->deselectAllNodes();
		QTreeWidgetItemIterator it(m_treeWidget.get(), QTreeWidgetItemIterator::Selected);
		bool hasSelected = false;
		while (*it)
		{
			SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
			shared_ptr<SceneNode> node = item->getSceneNode();
			m_context->getScene()->selectNode(node);
			hasSelected = true;
			it++;
		}

		m_deleteAction->setEnabled(hasSelected);
	}
}

void SceneGraphTree::_refreshNode(SceneNode *node)
{
	if (node != nullptr && m_currentNodes[node] != nullptr)
	{
		m_currentNodes[node]->setText(0, QString::fromStdString(node->getName()));
	}
}