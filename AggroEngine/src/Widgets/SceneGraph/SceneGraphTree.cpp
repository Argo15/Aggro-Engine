#include "SceneGraphTree.hpp"
#include "SceneTreeWidget.hpp"
#include "DirectLightRenderComponent.hpp"
#include <QPushButton>
#include <QAction>
#include <QMouseEvent>
#include <QModelIndex>
#include <QFileDialog>
#include <iostream>
#include <boost/unordered_map.hpp>

SceneGraphTree::SceneGraphTree(shared_ptr<EngineContext> context, QWidget *parent)
	: QDockWidget(parent)
	, m_treeWidget(shared_ptr<QTreeWidget>(new SceneTreeWidget()))
	, m_context(context)
{
	m_addCubeAction = new QAction(tr("Add Cube"), this);
	connect(m_addCubeAction, &QAction::triggered, this, [this]() {
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		renderComponent->setMeshId(m_context->getResources()->getIdForPath("Resources/Mesh/Engine/cube.obj"));
		_addNewNode(renderComponent, "Cube");
	});

	m_addSphereAction = new QAction(tr("Add Sphere"), this);
	connect(m_addSphereAction, &QAction::triggered, this, [this]() {
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		renderComponent->setMeshId(m_context->getResources()->getIdForPath("Resources/Mesh/Engine/sphere.obj"));
		_addNewNode(renderComponent, "Sphere");
	});

	QAction *addFromFileAction = new QAction(tr("Add From File"), this);
	connect(addFromFileAction, &QAction::triggered, this, [this]() {
		QDir workingDirectory = QDir::current();
		QString filename = QFileDialog::getOpenFileName(this, tr("Add Object From File"), workingDirectory.path() + "/Resources/Mesh");
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		renderComponent->setMeshId(m_context->getResources()->getIdForPath(workingDirectory.relativeFilePath(filename).toStdString()));
		_addNewNode(renderComponent, QFileInfo(filename).fileName().split(".").first().toStdString());
	});

	QAction *addSpriteAction = new QAction(tr("Add Sprite"), this);
	connect(addSpriteAction, &QAction::triggered, this, [this]() {
		QDir workingDirectory = QDir::current();
		QString filename = QFileDialog::getOpenFileName(this, tr("Add Sprite"), workingDirectory.path() + "/Resources/Textures");
		shared_ptr<SpriteRenderComponent> renderComponent(new SpriteRenderComponent(m_context->getResources()));
		shared_ptr<SceneNode> newNode = _addNewNode(renderComponent, "Sprite");
		if (newNode->getTransformComponent())
		{
			newNode->getTransformComponent()->setScale(glm::vec3(0.3, 0.3, 0.3));
		}
		newNode->setMaterialComponent(shared_ptr<MaterialComponent>(new MaterialComponent()));
		newNode->getMaterialComponent()->setTextureImageId(m_context->getResources()->
			getIdForPath((workingDirectory.relativeFilePath(filename).toStdString())));
	});

	QAction *addDirectLightAction = new QAction(tr("Add Direct Light"), this);
	connect(addDirectLightAction, &QAction::triggered, this, [this]() {
		shared_ptr<DirectLightRenderComponent> renderComponent(new DirectLightRenderComponent(m_context->getResources()));
		shared_ptr<SceneNode> newNode = _addNewNode(renderComponent, "Direct Light");
		newNode->setDirectLightComponent(shared_ptr<DirectLightComponent>(new DirectLightComponent()));
		if (newNode->getTransformComponent())
		{
			newNode->getTransformComponent()->setScale(glm::vec3(0.3, 0.3, 0.3));
			newNode->getTransformComponent()->setTranslate(glm::vec3(0, 3, 0));
			newNode->getTransformComponent()->setRotate(glm::vec3(0.52, 0, -0.52));
		}
		newNode->setMaterialComponent(shared_ptr<MaterialComponent>(new MaterialComponent()));
		newNode->getMaterialComponent()->setTextureImageId(
			m_context->getResources()->getIdForPath(DirectLightRenderComponent::s_imagePath));
	});

	m_deleteAction = new QAction(tr("Delete"), this);
	m_deleteAction->setEnabled(false);
	connect(m_deleteAction, &QAction::triggered, this, &SceneGraphTree::_deleteSelected);

	// Create new tree widget
	m_treeWidget = shared_ptr<QTreeWidget>(new SceneTreeWidget(this));
	m_treeWidget->setHeaderHidden(true);
	m_treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	m_treeWidget->addAction(m_addCubeAction);
	m_treeWidget->addAction(m_addSphereAction);
	m_treeWidget->addAction(addFromFileAction);
	m_treeWidget->addAction(addSpriteAction);
	m_treeWidget->addAction(addDirectLightAction);
	m_treeWidget->addAction(m_deleteAction);
	connect(m_treeWidget.get(), &QTreeWidget::itemSelectionChanged, this, &SceneGraphTree::_selectionChanged);
	
	m_context->getScene()->addUpdateListener([this](auto scene) {refresh(scene);});
	m_context->getScene()->addSelectionChangeListener([this](auto node) {_selectNode(node.get());});
	m_context->addNewSceneListener([this](auto scene) {
		m_treeWidget->clear();
		m_currentNodes = boost::unordered_map<SceneNode *, QTreeWidgetItem *>();
		refresh(scene);
		scene->addUpdateListener([this](auto scene) {refresh(scene); });
		scene->addSelectionChangeListener([this](auto node) {_selectNode(node.get()); });
	});
	setMouseTracking(true);
	setWidget(m_treeWidget.get());
}

void SceneGraphTree::refresh(Scene* scene)
{
	boost::lock_guard<SceneGraphTree> guard(*this);
	QSignalBlocker block(m_treeWidget.get());
	_addSceneNodeRecursive(scene->getRoot(), nullptr, true);
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
		if (treeItem != nullptr)
		{
			m_currentNodes[node.get()] = treeItem;
		}
		else
		{
			_deleteNode(node.get());
		}
		node->addChangeListener(this, [this](auto updateNode) {this->_refreshNode(updateNode);});
		node->addDeletedListener(this, [this](auto deleteNode) {this->_deleteNode(deleteNode);});
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

shared_ptr<SceneNode> SceneGraphTree::_addNewNode(shared_ptr<StaticObjectRenderComponent> renderComponent, string name)
{
	boost::lock_guard<SceneGraphTree> guard(*this);
	QTreeWidgetItemIterator it(m_treeWidget.get(), QTreeWidgetItemIterator::Selected);
	shared_ptr<SceneNode> node = m_context->getScene()->getRoot();

	if (*it)
	{
		SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
		node = item->getSceneNode();
	}

	//Create new scene node
	shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId(), node.get()));
	newNode->setRenderComponent(renderComponent);
	newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
	newNode->setName(name);
	node->addChild(newNode);

	// Update scene graph tree
	m_context->getScene()->deselectAllNodes();
	m_context->getScene()->selectNode(newNode);
	m_context->getScene()->update();

	return newNode;
}

void SceneGraphTree::_selectionChanged()
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

void SceneGraphTree::_deleteSelected()
{
	QTreeWidgetItemIterator it(m_treeWidget.get(), QTreeWidgetItemIterator::Selected);
	while (*it)
	{
		SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
		shared_ptr<SceneNode> node = item->getSceneNode();
		m_context->getScene()->deleteNode(node);
		it++;
	}
}

void SceneGraphTree::_refreshNode(SceneNode *node)
{
	boost::lock_guard<SceneGraphTree> guard(*this);
	if (node != nullptr && m_currentNodes[node] != nullptr)
	{
		m_currentNodes[node]->setText(0, QString::fromStdString(node->getName()));
	}
}

void SceneGraphTree::_deleteNode(SceneNode *node)
{
	boost::lock_guard<SceneGraphTree> guard(*this);
	if (node != nullptr && m_currentNodes[node] != nullptr)
	{
		QTreeWidgetItem *item = m_currentNodes[node];
		if (item != nullptr)
		{
			delete item;
			m_currentNodes[node] = nullptr;
		}
	}
}

void SceneGraphTree::_selectNode(SceneNode *node)
{
	QSignalBlocker block(m_treeWidget.get());
	if (node != nullptr && m_currentNodes[node] != nullptr)
	{
		QTreeWidgetItemIterator it(m_treeWidget.get(), QTreeWidgetItemIterator::Selected);
		while (*it)
		{
			SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
			item->setSelected(false);
			it++;
		}
		QTreeWidgetItem *parent = m_currentNodes[node]->parent();
		while (parent != nullptr)
		{
			parent->setExpanded(true);
			parent = parent->parent();
		}
		m_currentNodes[node]->setSelected(true);
	}
}

void SceneGraphTree::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete)
	{
		_deleteSelected();
	}
}