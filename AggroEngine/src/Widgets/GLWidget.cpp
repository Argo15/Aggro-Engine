#include "GLWidget.hpp"
#include "../Core/Object/Components/Render/StaticObjectRenderComponent.hpp"

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent)
	, m_graphics(shared_ptr<OpenGL43Graphics>(new OpenGL43Graphics()))
	, m_keyboard(shared_ptr<KeyboardState>(new KeyboardState()))
	, m_mouse(shared_ptr<MouseState>(new MouseState()))
	, m_cameraController(shared_ptr<CameraController>(new FreeRoamCameraController()))
	, m_nFPS(60)
	, m_meshImporter(shared_ptr<MeshImporter>(new AssimpMeshImporter()))
	, m_imageImporter(shared_ptr<ImageImporter>(new FreeImageImporter()))
{
	setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);
}

void GLWidget::initializeGL()
{
	glewInit();

	m_graphics->init();

	// Load temp resources
	shared_ptr<Mesh> mesh(m_meshImporter->importMesh("Resources/Mesh/sphere.obj"));
	shared_ptr<Image> image = m_imageImporter->importImage("Resources/Image/Banana2.png");
	shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(image));

	// create a temp root object
	shared_ptr<Object> rootObject(new Object());
	shared_ptr<StaticObjectRenderComponent> objectRenderComponent(new StaticObjectRenderComponent());
	objectRenderComponent->setVertexBuffer(m_graphics->createVertexBuffer(mesh));
	objectRenderComponent->setTexture(m_graphics->createTexture(texOptions));
	rootObject->setRenderComponent(objectRenderComponent);

	// create scene from temp object
	m_scene = shared_ptr<Scene>(new Scene(rootObject, shared_ptr<Camera>(new Camera())));

	setAutoBufferSwap(true);
}

void GLWidget::resizeGL(int width, int height)
{
	m_graphics->setViewport(0, 0, width, height);
	m_scene->getCamera()->setProjection(glm::perspective(45.f, (float)width / (float)height, 0.01f, 100.f));
}

void GLWidget::paintGL()
{
	m_cameraController->handleKeyboardInput(*m_scene->getCamera().get(), *m_keyboard.get(), 1.f / m_nFPS);

	static float rotate = 0.f;
	if (m_nFPS <= 0)
	{
		return;
	}
	m_scene->getRoot()->setTransform(glm::toMat4(glm::angleAxis(rotate, glm::normalize(glm::vec3(0.5f, 1.f, 0.7f)))));
	rotate += 120 / m_nFPS;

	// clear
	m_graphics->clearDepthAndColor();

	// Stage root node for render
	m_scene->getRoot()->getRenderComponent()->render(m_graphics, m_scene->getRoot()->getTransform());

	// set scene options
	RenderOptions renderOptions;
	renderOptions.setProjectionMatrix(m_scene->getCamera()->getProjMatrix());
	renderOptions.setViewMatrix(m_scene->getCamera()->getViewMatrix());

	// execute
	m_graphics->executeRender(renderOptions);
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