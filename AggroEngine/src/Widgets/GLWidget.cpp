#include "GLWidget.hpp"
#include "../Core/Object/Components/Render/StaticObjectRenderComponent.hpp"

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent)
	, m_graphics(shared_ptr<OpenGL43Graphics>(new OpenGL43Graphics()))
	, m_keyboard(shared_ptr<KeyboardState>(new KeyboardState()))
	, m_mouse(shared_ptr<MouseState>(new MouseState()))
	, m_cameraController(shared_ptr<CameraController>(new FreeRoamCameraController()))
	, m_nFPS(60)
	, m_vboCache(shared_ptr<VertexBufferCache>(new VertexBufferCache(m_graphics)))
	, m_textureCache(shared_ptr<TextureCache>(new TextureCache(m_graphics)))
{
	m_renderer = shared_ptr<Renderer>(new Renderer(m_graphics));
	setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);
}

void GLWidget::initializeGL()
{
	glewInit();

	m_graphics->init();
	m_renderer->init(m_vboCache, m_textureCache);

	// create a temp root object
	shared_ptr<Object> rootObject(new Object());
	shared_ptr<StaticObjectRenderComponent> objectRenderComponent(new StaticObjectRenderComponent());
	objectRenderComponent->setVertexBuffer(m_vboCache->getVertexBuffer("Resources/Mesh/sphere.obj"));
	objectRenderComponent->setTexture(m_textureCache->getTexture("Resources/Image/Banana2.png"));
	rootObject->setRenderComponent(objectRenderComponent);

	shared_ptr<SceneNode> rootNode = shared_ptr<SceneNode>(new SceneNode(rootObject));

	// create scene from temp object
	m_scene = shared_ptr<Scene>(new Scene(rootNode, shared_ptr<Camera>(new Camera())));

	setAutoBufferSwap(true);
}

void GLWidget::resizeGL(int width, int height)
{
	m_graphics->setViewport(0, 0, width, height);
	m_scene->getCamera()->setProjection(glm::perspective(45.f, (float)width / (float)height, 0.01f, 100.f));
}

void GLWidget::paintGL()
{
	if (m_nFPS <= 0)
	{
		return;
	}
	m_cameraController->handleKeyboardInput(*m_scene->getCamera().get(), *m_keyboard.get(), 1.f / m_nFPS);
	m_renderer->renderScene(m_scene);
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
	m_cameraController->handleMouseInput(*m_scene->getCamera(), *m_mouse.get(), 1.f / m_nFPS);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	m_mouse->setScroll(m_mouse->getScroll() + event->delta());
}

void GLWidget::setFPS(int fps)
{
	m_nFPS = fps;
}