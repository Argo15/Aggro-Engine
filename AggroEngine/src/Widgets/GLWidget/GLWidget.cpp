#include "GLWidget.hpp"
#include "StaticObjectRenderComponent.hpp"
#include "Config.hpp"
#include "CameraUpdateJob.hpp"

GLWidget::GLWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QGLWidget(parent)
	, m_keyboard(new KeyboardState())
	, m_mouse(new MouseState())
	, m_mouseController(new MouseController())
	, m_context(context)
{
	m_renderer = shared_ptr<Renderer>(new Renderer(m_context->getGraphics()));
	setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);
	const Properties& props = gConfig->getProperties();
	QGLFormat format;
	if (!props.getBooleanProperty("graphics.vsync_enabled"))
	{
		format.setSwapInterval(0);
	}
	this->setFormat(format);
}

void GLWidget::initializeGL()
{
	glewInit();

	m_context->getGraphics()->init();
	m_renderer->init(m_context->getVboCache(), m_context->getTextureCache());

	m_context->getJobManager()->runJob(new CameraUpdateJob(
		m_context,
		shared_ptr<CameraController>(new FreeRoamCameraController()),
		m_keyboard,
		m_mouse
	));

	// create a root object
	shared_ptr<Object> child1Object(new Object());
	shared_ptr<Object> child2Object(new Object());
	shared_ptr<StaticObjectRenderComponent> objectRenderComponent(new StaticObjectRenderComponent());
	objectRenderComponent->setVertexBuffer(m_context->getVboCache()->getVertexBuffer("Resources/Mesh/sphere.obj"));
	objectRenderComponent->setTexture(m_context->getTextureCache()->getTexture("Resources/Textures/Walls/wall01/wall01_Diffuse.tga"));
	child1Object->setRenderComponent(objectRenderComponent);
	child1Object->translate(glm::vec3(3.0, 0, 0));
	child2Object->setRenderComponent(objectRenderComponent);
	child2Object->translate(glm::vec3(0, 0, -3.0));

	shared_ptr<Scene> scene = m_context->getScene();
	shared_ptr<SceneNode> rootNode = shared_ptr<SceneNode>(new SceneNode(scene->getNextId()));
	shared_ptr<SceneNode> child1 = shared_ptr<SceneNode>(new SceneNode(scene->getNextId(), rootNode.get(), child1Object));
	rootNode->addChild(child1);
	shared_ptr<SceneNode> child2 = shared_ptr<SceneNode>(new SceneNode(scene->getNextId(), rootNode.get(), child2Object));
	rootNode->addChild(child2);

	// set initial scene
	scene->setRoot(rootNode);
	scene->setCamera(shared_ptr<Camera>(new Camera()));
	scene->update(); // Alert all listeners

	setAutoBufferSwap(true);
}

void GLWidget::resizeGL(int width, int height)
{
	m_context->getGraphics()->setViewport(0, 0, width, height);
	m_context->getScene()->getCamera()->setProjection(glm::perspective(45.f, (float)width / (float)height, 0.01f, 100.f));
}

void GLWidget::paintGL()
{
	if (m_context->getFPS() <= 0)
	{
		return;
	}
	m_mouseController->handleMouseInput(m_mouse, m_context);
	m_renderer->renderScene(m_context->getScene(), m_context->getRenderOptions());
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	m_keyboard->setKeyPressed(event->key(), true);
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