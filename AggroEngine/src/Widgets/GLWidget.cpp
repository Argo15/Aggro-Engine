#include "GLWidget.hpp"
#include "StaticObjectRenderComponent.hpp"

GLWidget::GLWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QGLWidget(parent)
	, m_keyboard(shared_ptr<KeyboardState>(new KeyboardState()))
	, m_mouse(shared_ptr<MouseState>(new MouseState()))
	, m_cameraController(shared_ptr<CameraController>(new FreeRoamCameraController()))
	, m_context(context)
{
	m_renderer = shared_ptr<Renderer>(new Renderer(m_context->getGraphics()));
	setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);
}

void GLWidget::initializeGL()
{
	glewInit();

	m_context->getGraphics()->init();
	m_renderer->init(m_context->getVboCache(), m_context->getTextureCache());

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

	shared_ptr<SceneNode> rootNode = shared_ptr<SceneNode>(new SceneNode());
	shared_ptr<SceneNode> child1 = shared_ptr<SceneNode>(new SceneNode(rootNode.get(), child1Object));
	rootNode->addChild(child1);
	shared_ptr<SceneNode> child2 = shared_ptr<SceneNode>(new SceneNode(rootNode.get(), child2Object));
	rootNode->addChild(child2);

	// set initial scene
	m_context->getScene()->setRoot(rootNode);
	m_context->getScene()->setCamera(shared_ptr<Camera>(new Camera()));
	m_context->getScene()->update(); // Alert all listeners

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
	m_cameraController->handleKeyboardInput(*m_context->getScene()->getCamera().get(), *m_keyboard.get(), 1.f / m_context->getFPS());
	m_renderer->renderScene(m_context->getScene());
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
	m_cameraController->handleMouseInput(*m_context->getScene()->getCamera(), *m_mouse.get(), 1.f / m_context->getFPS());
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	m_mouse->setScroll(m_mouse->getScroll() + event->delta());
}