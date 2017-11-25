#include "GLWidget.hpp"
#include "StaticObjectRenderComponent.hpp"
#include "Config.hpp"
#include "CameraUpdateJob.hpp"

GLWidget::GLWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QGLWidget(parent)
	, m_keyboard(new KeyboardState())
	, m_mouse(new MouseState())
	, m_mouseController(new MouseController())
	, m_engineContext(context)
	, m_graphicsClock(new Clock())
	, m_selection(new Selection())
{
	m_graphicsContext = shared_ptr<GraphicsContext>(new GraphicsContext(context->getJobManager(), context->getResources()));
	m_renderer = shared_ptr<Renderer>(new Renderer(m_graphicsContext));

	setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);
	const Properties& props = gConfig->getProperties();
	QGLFormat format;
	if (!props.getBooleanProperty("graphics.vsync_enabled"))
	{
		format.setSwapInterval(0);
	}
	m_maxFps = props.getIntProperty("graphics.max_fps");
	m_millisPerFrame = 1000 / m_maxFps;
	this->setFormat(format);
}

void GLWidget::initializeGL()
{
	glewInit();

	m_graphicsContext->getGraphics()->init();
	m_renderer->init();

	shared_ptr<Scene> scene = m_engineContext->getScene();

	// create a root object
	shared_ptr<SceneNode> rootNode = shared_ptr<SceneNode>(new SceneNode(scene->getNextId()));
	shared_ptr<SceneNode> child1 = shared_ptr<SceneNode>(new SceneNode(scene->getNextId(), rootNode.get()));
	rootNode->addChild(child1);
	shared_ptr<SceneNode> child2 = shared_ptr<SceneNode>(new SceneNode(scene->getNextId(), rootNode.get()));
	rootNode->addChild(child2);

	// add render data
	shared_ptr<StaticObjectRenderComponent> objectRenderComponent(new StaticObjectRenderComponent());
	objectRenderComponent->setMeshId(m_engineContext->getResources()->getIdForPath("Resources/Mesh/sphere.obj"));
	objectRenderComponent->setTextureImageId(m_engineContext->getResources()->getIdForPath("Resources/Textures/Walls/wall01/wall01_Diffuse.tga"));
	child1->setRenderComponent(objectRenderComponent);
	child2->setRenderComponent(objectRenderComponent);

	// add transform data
	shared_ptr<TransformComponent> transform(new TransformComponent());
	transform->translate(glm::vec3(3.0, 0, 0));
	transform->rotate(0.79f, glm::vec3(1.0, 0, 0));
	transform->rotate(0.79f, glm::vec3(0, 1.0, 0));
	transform->scale(glm::vec3(2.0, 2.0, 2.0));
	child1->setTransformComponent(transform);
	child1->setTransformComponent(transform);
	transform = shared_ptr<TransformComponent>(new TransformComponent());
	transform->translate(glm::vec3(0, 0, -3.0));
	child2->setTransformComponent(transform);
	child2->setTransformComponent(transform);

	// set initial scene
	scene->setRoot(rootNode);
	scene->setCamera(shared_ptr<Camera>(new Camera()));
	scene->update(); // Alert all listeners

	(new CameraUpdateJob(scene->getCamera(), shared_ptr<CameraController>(new FreeRoamCameraController()), m_keyboard, m_mouse))->run();

	setAutoBufferSwap(false);
}

void GLWidget::resizeGL(int width, int height)
{
	shared_ptr<Camera> camera = m_engineContext->getScene()->getCamera();
	camera->setViewport(glm::vec4(0, 0, width, height));
	camera->setProjection(glm::perspective(45.f, (float)width / (float)height, 0.01f, 100.f));
}

void GLWidget::paintGL()
{
	m_engineContext->getJobManager()->tick();
	shared_ptr<Job> graphicsJob;

	// If enough time has passed, render a new frame
	if (m_graphicsClock->getTimerMillis() >= m_millisPerFrame)
	{
		m_graphicsClock->resetTimer();
		m_renderer->renderScene(m_engineContext->getScene(), m_engineContext->getRenderOptions());
		swapBuffers();
		m_selection->updateSelection(m_mouse, m_graphicsContext->getGraphics());
		m_mouseController->handleMouseInput(m_mouse, m_engineContext, m_selection);

		// Process at least one graphics job
		if (graphicsJob = m_engineContext->getJobManager()->nextGraphicsJob())
		{
			graphicsJob->runInThread(); 
		}
	}

	// Process any jobs that require opengl (uploading images, VBOs, etc)
	while (m_graphicsClock->getTimerMillis() + 1 < m_millisPerFrame &&
			(graphicsJob = m_engineContext->getJobManager()->nextGraphicsJob()))
	{
		graphicsJob->runInThread();
	}
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