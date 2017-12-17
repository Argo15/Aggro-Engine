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
	, m_cameraUpdateJob()
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
	scene->update(); // notify listners

	m_cameraUpdateJob = _setupCameraUpdateJob(scene->getCamera());

	m_engineContext->addNewSceneListener([this](auto scene) { 
		this->resizeGL(width(), height());
		m_cameraUpdateJob->stop();
		m_cameraUpdateJob = _setupCameraUpdateJob(scene->getCamera());
	});

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

shared_ptr<Job> GLWidget::_setupCameraUpdateJob(shared_ptr<Camera> camera)
{
	shared_ptr<Job> newJob = shared_ptr<Job>(new CameraUpdateJob(
		camera,
		shared_ptr<CameraController>(new FreeRoamCameraController()),
		m_keyboard, m_mouse));
	newJob->run();
	return newJob;
}