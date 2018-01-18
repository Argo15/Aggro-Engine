#include "SceneGraphTree.hpp"
#include "SceneTreeWidget.hpp"
#include "DirectLightRenderComponent.hpp"
#include "FileBackedMesh.hpp"
#include <QPushButton>
#include <QAction>
#include <QMouseEvent>
#include <QModelIndex>
#include <QFileDialog>
#include <iostream>
#include <boost/unordered_map.hpp>

SceneGraphTree::SceneGraphTree(shared_ptr<EngineContext> context, QWidget *parent)
	: QDockWidget(parent)
	, m_treeWidget(shared_ptr<QTreeWidget>(new SceneTreeWidget(this)))
	, m_context(context)
	, m_materialsItem(nullptr)
{
	shared_ptr<Resources> resources = m_context->getResources();
	shared_ptr<MeshImporter> meshImporter = m_context->getMeshImporter();

	m_addCubeAction = new QAction(tr("Add Cube"), this);
	connect(m_addCubeAction, &QAction::triggered, this, [this, resources, meshImporter]() {
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		newNode->setRenderComponent(renderComponent);
		newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
		newNode->setName("Cube");
		shared_ptr<MeshComponent> meshComponent(new MeshComponent());
		int meshId = resources->getIdForPath("Resources/Mesh/Engine/cube.obj");
		meshComponent->setPrimaryMesh(shared_ptr<Mesh>(new FileBackedMesh(meshId, resources, meshImporter)));
		newNode->setMeshComponent(meshComponent);
		_addNewNode(newNode);
	});

	m_addSphereAction = new QAction(tr("Add Sphere"), this);
	connect(m_addSphereAction, &QAction::triggered, this, [this, resources, meshImporter]() {
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		newNode->setRenderComponent(renderComponent);
		newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
		newNode->setName("Sphere");
		shared_ptr<MeshComponent> meshComponent(new MeshComponent());
		int meshId = resources->getIdForPath("Resources/Mesh/Engine/sphere.obj");
		meshComponent->setPrimaryMesh(shared_ptr<Mesh>(new FileBackedMesh(meshId, resources, meshImporter)));
		newNode->setMeshComponent(meshComponent);
		_addNewNode(newNode);
	});

	QAction *addFromFileAction = new QAction(tr("Add From File"), this);
	connect(addFromFileAction, &QAction::triggered, this, [this, resources, meshImporter]() {
		QDir workingDirectory = QDir::current();
		QString filename = QFileDialog::getOpenFileName(this, tr("Add Object From File"), workingDirectory.path() + "/Resources/Mesh");
		shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
		shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		newNode->setRenderComponent(renderComponent);
		newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
		newNode->setName(QFileInfo(filename).fileName().split(".").first().toStdString());
		shared_ptr<MeshComponent> meshComponent(new MeshComponent());
		int meshId = resources->getIdForPath(workingDirectory.relativeFilePath(filename).toStdString());
		meshComponent->setPrimaryMesh(shared_ptr<Mesh>(new FileBackedMesh(meshId, resources, meshImporter)));
		newNode->setMeshComponent(meshComponent);
		_addNewNode(newNode);
	});

	QAction *addSpriteAction = new QAction(tr("Add Sprite"), this);
	connect(addSpriteAction, &QAction::triggered, this, [this, resources, meshImporter]() {
		QDir workingDirectory = QDir::current();
		QString filename = QFileDialog::getOpenFileName(this, tr("Add Sprite"), workingDirectory.path() + "/Resources/Textures");
		shared_ptr<SpriteRenderComponent> renderComponent(new SpriteRenderComponent(m_context->getResources()));
		shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		newNode->setRenderComponent(renderComponent);
		newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
		newNode->setName("Sprite");
		newNode->getTransformComponent()->setScale(glm::vec3(0.3, 0.3, 0.3));
		newNode->setMaterialComponent(shared_ptr<MaterialComponent>(new MaterialComponent(newNode.get())));
		newNode->getMaterialComponent()->setTextureImageId(m_context->getResources()->
			getIdForPath((workingDirectory.relativeFilePath(filename).toStdString())));
		shared_ptr<MeshComponent> meshComponent(new MeshComponent());
		int meshId = resources->getIdForPath("Resources/Mesh/Engine/Plane.obj");
		meshComponent->setPrimaryMesh(shared_ptr<Mesh>(new FileBackedMesh(meshId, resources, meshImporter)));
		newNode->setMeshComponent(meshComponent);
		_addNewNode(newNode);
	});

	QAction *addDirectLightAction = new QAction(tr("Add Direct Light"), this);
	connect(addDirectLightAction, &QAction::triggered, this, [this, resources, meshImporter]() {
		shared_ptr<DirectLightRenderComponent> renderComponent(new DirectLightRenderComponent(m_context->getResources()));
		shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		newNode->setRenderComponent(renderComponent);
		newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
		newNode->setName("Direct Light");
		newNode->setDirectLightComponent(shared_ptr<DirectLightComponent>(new DirectLightComponent()));
		newNode->getTransformComponent()->setScale(glm::vec3(0.3, 0.3, 0.3));
		newNode->getTransformComponent()->setTranslate(glm::vec3(0, 3, 0));
		newNode->getTransformComponent()->setRotate(glm::vec3(0.52, 0, -0.52));
		newNode->setMaterialComponent(shared_ptr<MaterialComponent>(new MaterialComponent(newNode.get())));
		newNode->getMaterialComponent()->setTextureImageId(
			m_context->getResources()->getIdForPath(DirectLightRenderComponent::s_imagePath));
		newNode->getMaterialComponent()->setAlphaImageId(
			m_context->getResources()->getIdForPath(DirectLightRenderComponent::s_alphaPath));
		shared_ptr<MeshComponent> meshComponent(new MeshComponent());
		int meshId = resources->getIdForPath("Resources/Mesh/Engine/Plane.obj");
		meshComponent->setPrimaryMesh(shared_ptr<Mesh>(new FileBackedMesh(meshId, resources, meshImporter)));
		newNode->setMeshComponent(meshComponent);
		_addNewNode(newNode);
	});

	QAction *addMaterial = new QAction(tr("Add Material"), this);
	connect(addMaterial, &QAction::triggered, this, [this]() {
		shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		newNode->setMaterialComponent(shared_ptr<MaterialComponent>(new MaterialComponent(newNode.get())));
		newNode->setName("New Material");
		newNode->setBaseMaterialNode();
		m_context->getScene()->addBaseMaterial(newNode);
		m_context->getScene()->selectNodeById(newNode->getId());
		refresh(m_context->getScene().get());
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
	m_treeWidget->addAction(addMaterial);
	m_treeWidget->addAction(m_deleteAction);
	connect(m_treeWidget.get(), &QTreeWidget::itemSelectionChanged, this, &SceneGraphTree::_selectionChanged);
	
	m_context->getScene()->addUpdateListener([this](auto scene) {refresh(scene);});
	m_context->getScene()->addSelectionChangeListener([this](auto node) {_selectNode(node.get());});
	m_context->addNewSceneListener([this](auto scene) {
		m_materialsItem = nullptr;
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

	boost::unordered_map<int, shared_ptr<SceneNode>> matNodes = scene->getBaseMaterials();
	if (m_materialsItem != nullptr)
	{
		delete m_materialsItem;
	}
	if (!matNodes.empty())
	{
		m_materialsItem = new SceneNodeTreeItem(shared_ptr<SceneNode>(), m_treeWidget.get());
		m_materialsItem->setText(0, "Materials");
		m_materialsItem->setFont(0, QFont("Times", -1, QFont::Bold));
		m_materialsItem->setExpanded(true);
		for (auto matNode : matNodes | boost::adaptors::map_values)
		{
			SceneNodeTreeItem *treeItem = new SceneNodeTreeItem(matNode);
			treeItem->setText(0, QString::fromStdString(matNode->getName()));
			m_currentNodes[matNode.get()] = treeItem;
			m_materialsItem->addChild(treeItem);
			matNode->addChangeListener(this, [this](auto updateNode) {this->_refreshNode(updateNode); });
			matNode->addDeletedListener(this, [this](auto deleteNode) {this->_deleteNode(deleteNode); });
			if (scene->getSelectedNode() == matNode)
			{
				treeItem->setSelected(true);
			}
		}
	}
}

void SceneGraphTree::_addSceneNodeRecursive(shared_ptr<SceneNode> node, QTreeWidgetItem *parent, bool isRoot)
{
	if (!node)
	{
		return;
	}
	if (parent == m_materialsItem)
	{
		parent = nullptr;
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

void SceneGraphTree::_addNewNode(shared_ptr<SceneNode> newNode)
{
	boost::lock_guard<SceneGraphTree> guard(*this);
	QTreeWidgetItemIterator it(m_treeWidget.get(), QTreeWidgetItemIterator::Selected);
	shared_ptr<SceneNode> node = m_context->getScene()->getRoot();

	if (*it)
	{
		SceneNodeTreeItem *item = (SceneNodeTreeItem *)(*it);
		if (item->getSceneNode())
		{
			node = item->getSceneNode();
		}
	}

	// Add new scene node
	node->addChild(newNode);

	// Update scene graph tree
	m_context->getScene()->deselectAllNodes();
	m_context->getScene()->selectNode(newNode);
	m_context->getScene()->update();
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
		if (node)
		{
			m_context->getScene()->selectNode(node);
			hasSelected = true;
		}
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
		if (node)
		{
			m_context->getScene()->deleteNode(node);
		}
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