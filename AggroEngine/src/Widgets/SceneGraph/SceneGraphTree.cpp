#include "SceneGraphTree.hpp"
#include "SceneTreeWidget.hpp"
#include "StaticObjectRenderComponent.hpp"
#include <QPushButton>
#include <QAction>
#include <QMouseEvent>
#include <QModelIndex>
#include <iostream>

SceneGraphTree::SceneGraphTree(shared_ptr<EngineContext> context, QWidget *parent)
	: QDockWidget(parent)
	, m_treeWidget(shared_ptr<QTreeWidget>(new SceneTreeWidget()))
	, m_context(context)
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

	refresh(m_context->getScene().get());
	m_context->getScene()->addUpdateListener([this](auto scene) {refresh(scene);});
	setMouseTracking(true);
}

void SceneGraphTree::refresh(Scene* scene)
{
	// Create new tree widget
	m_treeWidget = shared_ptr<QTreeWidget>(new SceneTreeWidget(this));
	m_treeWidget->setHeaderHidden(true);
	m_treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	m_treeWidget->addAction(m_addCubeAction);
	m_treeWidget->addAction(m_addSphereAction);
	connect(m_treeWidget.get(), &QTreeWidget::itemSelectionChanged, this, &SceneGraphTree::_selectionChanged);

	_addSceneNodeRecursive(scene->getRoot(), nullptr, true);
}

void SceneGraphTree::_addSceneNodeRecursive(shared_ptr<SceneNode> node, QTreeWidgetItem *parent, bool isRoot)
{
	if (!node)
	{
		return;
	}
	// Create item for current
	QTreeWidgetItem *treeItem = nullptr;
	if (isRoot)
	{
		setWindowTitle("Scene");
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

	// Recursive iterate all children
	shared_ptr<vector<shared_ptr<SceneNode>>> children = node->getChildren();
	if (children->size() > 0)
	{
		for (auto &child : *children.get())
		{
			_addSceneNodeRecursive(child, treeItem, false);
		}
	}

	setWidget(m_treeWidget.get());
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
	QTreeWidgetItem *newItem;
	if (*it)
	{
		newItem = new SceneNodeTreeItem(newNode);
		SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
		item->addChild(newItem);
		item->setExpanded(true);
		item->setSelected(false);
	}
	else
	{
		newItem = new SceneNodeTreeItem(newNode, m_treeWidget.get());
	}
	newItem->setSelected(true);
	newItem->setText(0, QString::fromStdString(newNode->getName()));
}

void SceneGraphTree::_selectionChanged()
{
	m_context->getScene()->deselectAllNodes();
	QTreeWidgetItemIterator it(m_treeWidget.get(), QTreeWidgetItemIterator::Selected);
	while (*it)
	{
		SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
		shared_ptr<SceneNode> node = item->getSceneNode();
		node->setSelected(true);
		it++;
	}
}