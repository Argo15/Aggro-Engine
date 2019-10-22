#include "GLWidget.hpp"
#include <QDrag>
#include <QMimeData>
#include <QOpenGLContext>
#include "StaticObjectRenderComponent.hpp"
#include "Config.hpp"
#include "CameraUpdateJob.hpp"
#include "LineMesh.hpp"
#include "PerfStats.hpp"

const shared_ptr<Mesh> previewMesh = shared_ptr<Mesh>(new LineMesh(-1, glm::vec3(0), glm::vec3(0, 1.0, 0)));

GLWidget::GLWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QOpenGLWidget(parent)
	, m_keyboard(new KeyboardState())
	, m_mouse(new MouseState())
	, m_mouseController(new MouseController())
	, m_engineContext(context)
	, m_graphicsClock(new Clock())
	, m_selection(new Selection())
	, m_cameraUpdateJob()
{
	m_graphicsContext = shared_ptr<GraphicsContext>(new GraphicsContext(context->getJobManager(), context->getResources()));
	m_renderer = shared_ptr<Renderer>(new Renderer(m_graphicsContext));

	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	const Properties& props = Config::instance().getProperties();
	m_maxFps = props.getIntProperty("graphics.max_fps");
	m_millisPerFrame = 1000 / m_maxFps;
	setAcceptDrops(true);
}

void GLWidget::initializeGL()
{
	glewExperimental = true;
	glewInit(); 

	m_graphicsContext->getGraphics()->init(shared_ptr<GraphicsInitOptions>(new GraphicsInitOptions()));
	m_renderer->init();

	shared_ptr<Scene> scene = m_engineContext->getScene();
	scene->update(); // notify listners

	m_cameraUpdateJob = _setupCameraUpdateJob(scene->getCameraNode());

	m_engineContext->addNewSceneListener([this](auto scene) { 
		_refreshScene(scene); 
		scene->addCameraChangeListener([this](auto camera) { _refreshCamera(camera); });
	});
	scene->addCameraChangeListener([this](auto camera) { _refreshCamera(camera); });
}

void GLWidget::resizeGL(int width, int height)
{
	m_engineContext->getScene()->applyToAllNodes([width, height](auto node) {
		if (node->getCameraComponent())
		{
			node->getCameraComponent()->setViewport(glm::vec4(0, 0, width, height));
			node->getCameraComponent()->setProjection(45.f, (float)width / (float)height, 0.01f, 100.f);
		}
	});
}

void GLWidget::paintGL()
{
	auto methodTracker = PerfStats::instance().trackTime("paintGL");
	m_engineContext->getJobManager()->tick();
	shared_ptr<Job> graphicsJob;
	shared_ptr<RenderOptions> renderOptions = m_engineContext->getRenderOptions();
	renderOptions->setDefaultFrameBufferId(defaultFramebufferObject());

	// If enough time has passed, render a new frame
	if (m_graphicsClock->getTimerMillis() >= m_millisPerFrame)
	{
		// This flushes GL calls due to glReadPixels, so use the already finished scene
		// TODO: do asyncronously using pixel buffer objects
		m_selection->updateSelection(m_mouse, m_graphicsContext->getGraphics());

		m_graphicsClock->resetTimer();
		m_renderer->renderScene(m_engineContext->getScene(), renderOptions);
		PerfStats::instance().recordFrame();
		m_mouseController->handleMouseInput(m_mouse, m_engineContext, m_selection);
		m_engineContext->getScene()->update(m_selection, m_mouse, m_engineContext);

		// Process a few graphics jobs no matter what
		int numJobs = 0;
		while (graphicsJob = m_engineContext->getJobManager()->nextGraphicsJob())
		{
			auto tracker = PerfStats::instance().trackTime("Graphics Job");
			graphicsJob->runInThread();
			if (numJobs++ >= 5)
			{
				break;
			}
		}
	}

	// Process any jobs that require opengl (uploading images, VBOs, etc)
	while (m_graphicsClock->getTimerMillis() + 1 < m_millisPerFrame &&
			(graphicsJob = m_engineContext->getJobManager()->nextGraphicsJob()))
	{
		auto tracker = PerfStats::instance().trackTime("Graphics Job");
		graphicsJob->runInThread();
	}

	m_graphicsContext->getGraphics()->drawScreen(*renderOptions);
}

shared_ptr<GraphicsContext> GLWidget::getGraphicsContext()
{
	return m_graphicsContext;
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	m_keyboard->setKeyPressed(event->key(), true);
	if (event->key() == Qt::Key_Delete)
	{
		m_engineContext->getScene()->deleteSelectedNode();
	}
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
	m_keyboard->setKeyPressed(event->key(), false);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	m_mouse->setButtonPressed(event->button(), true);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouse->setButtonPressed(event->button(), false);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	m_mouse->setPosition(event->x(), event->y());
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	m_mouse->setScroll(m_mouse->getScroll() + event->delta());
}

void GLWidget::_refreshScene(Scene *scene)
{
	this->resizeGL(width(), height());
	_refreshCamera(scene->getCameraNode());
}

void GLWidget::_refreshCamera(shared_ptr<SceneNode> camera)
{
	if (m_cameraUpdateJob)
	{
		m_cameraUpdateJob->stop();
	}
	m_cameraUpdateJob = _setupCameraUpdateJob(camera);
}

shared_ptr<Job> GLWidget::_setupCameraUpdateJob(shared_ptr<SceneNode> camera)
{
	shared_ptr<Job> newJob = shared_ptr<Job>(new CameraUpdateJob(
		camera,
		shared_ptr<CameraController>(new FreeRoamCameraController()),
		m_keyboard, m_mouse));
	newJob->run();
	return newJob;
}

void GLWidget::dragMoveEvent(QDragMoveEvent *event)
{
	m_mouse->setPosition(event->pos().x(), event->pos().y());
	if (event->mimeData()->hasFormat("application/x-materialdata") ||
		event->mimeData()->hasFormat("application/x-texture"))
	{
		int selectedId = m_selection->getSelectionAsId();
		shared_ptr<SceneNode> node = m_engineContext->getScene()->getNodeById(selectedId);
		if (node)
		{
			event->acceptProposedAction();
			return;
		}
	}
	else if (event->mimeData()->hasFormat("application/x-mesh"))
	{
		event->acceptProposedAction();
		return;
	}
	event->ignore();
}

void GLWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-materialdata") ||
		event->mimeData()->hasFormat("application/x-texture") ||
		event->mimeData()->hasFormat("application/x-mesh"))
	{
		if (event->mimeData()->hasFormat("application/x-mesh"))
		{
			_getPreviewNode()->getMeshComponent()->setPrimaryMesh(previewMesh);
		}
		event->acceptProposedAction();
	}
}

void GLWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
	if (m_engineContext->getScene()->getPreviewNode())
	{
		_getPreviewNode()->getMeshComponent()->setPrimaryMesh(shared_ptr<Mesh>());
	}
	event->accept();
}

void GLWidget::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-materialdata"))
	{
		shared_ptr<SceneNode> matNode = m_engineContext->getScene()->getSelectedNode();
		int selectedId = m_selection->getSelectionAsId();
		shared_ptr<SceneNode> dropNode = m_engineContext->getScene()->getNodeById(selectedId);
		if (matNode && dropNode)
		{
			dropNode->setMaterialComponent(matNode->getMaterialComponent());
		}
	}
	else if (event->mimeData()->hasFormat("application/x-texture"))
	{
		_dropTexture(event);
	}
	else if (event->mimeData()->hasFormat("application/x-mesh"))
	{
		_dropMesh(event);
	}
	event->accept();
}

void GLWidget::_dropTexture(QDropEvent *event)
{
	m_mouse->setPosition(event->pos().x(), event->pos().y());
	int selectedId = m_selection->getSelectionAsId();
	shared_ptr<SceneNode> dropNode = m_engineContext->getScene()->getNodeById(selectedId);
	if (dropNode)
	{
		string filename = event->mimeData()->data("application/x-texture").toStdString();
		int imageId = m_engineContext->getResources()->getIdForPath(filename);
		shared_ptr<MaterialComponent> material = dropNode->getMaterialComponent();
		if (material && material->getOwner() == dropNode.get())
		{
			material->setTextureImageId(imageId);
		}
		else
		{
			if (material)
			{
				material = shared_ptr<MaterialComponent>(new MaterialComponent(dropNode.get(), material.get()));
			}
			else
			{
				material = shared_ptr<MaterialComponent>(new MaterialComponent(dropNode.get()));
			}
			material->setTextureImageId(imageId);
			dropNode->setMaterialComponent(material);
		}
	}
}

void GLWidget::_dropMesh(QDropEvent *event)
{
	QString filepath = event->mimeData()->data("application/x-mesh");
	QString name = filepath.split("/").last().split(".").first();
	shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
	shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
	newNode->setRenderComponent(renderComponent);
	newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
	newNode->setName(name.toStdString());
	shared_ptr<MeshComponent> meshComponent(new MeshComponent(m_engineContext->getJobManager()));
	int meshId = m_engineContext->getResources()->getIdForPath(filepath.toStdString());
	m_engineContext->getMeshCache()->getMesh(meshId)->onReady([this, meshComponent, newNode](auto mesh) {
		glm::vec3 intersectPos = _getPreviewNode()->getTransformComponent()->getTranslate();
		auto metadata = mesh->getMetaData();
		newNode->getTransformComponent()->setTranslate(intersectPos + glm::vec3(0, metadata->getYLength() / 2.0, 0));
		meshComponent->setPrimaryMesh(mesh);
		meshComponent->setAxisAlign(MeshComponent::AxisAlign::CENTER);
	});
	newNode->setMeshComponent(meshComponent);
	m_engineContext->getScene()->getRoot()->addChild(newNode);
	m_engineContext->getScene()->deselectAllNodes();
	m_engineContext->getScene()->selectNode(newNode);
	m_engineContext->getScene()->update();

	_getPreviewNode()->getMeshComponent()->setPrimaryMesh(shared_ptr<Mesh>());
}

shared_ptr<SceneNode> GLWidget::_getPreviewNode()
{
	shared_ptr<SceneNode> previewNode = m_engineContext->getScene()->getPreviewNode();
	if (!previewNode)
	{
		previewNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		m_engineContext->getScene()->setPreviewNode(previewNode);
	}
	if (!previewNode->getTransformComponent())
	{
		previewNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
	}
	if (!previewNode->getRenderComponent())
	{
		previewNode->setRenderComponent(shared_ptr<StaticObjectRenderComponent>(new StaticObjectRenderComponent()));
	}
	shared_ptr<MeshComponent> meshComp = previewNode->getMeshComponent();
	if (!meshComp)
	{
		meshComp = shared_ptr<MeshComponent>(new MeshComponent(m_engineContext->getJobManager()));
		previewNode->setMeshComponent(meshComp);
		meshComp->enableTangents(false);
	}
	return previewNode;
}