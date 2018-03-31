#include "GLMeshWidget.hpp"
#include "CubeMesh.hpp"
#include "Config.hpp"
#include <QOpenGLContext>
#include <QDir>

GLMeshWidget::GLMeshWidget(QString &path, shared_ptr<EngineContext> context, QWidget *parent)
	: QOpenGLWidget(parent)
	, m_engineContext(context)
	, m_camera(new SceneNode(-1))
	, m_initialized(false)
{
	m_graphicsContext = shared_ptr<GraphicsContext>(new GraphicsContext(context->getJobManager(), context->getResources()));

	shared_ptr<CameraComponent> camera = m_camera->getCameraComponent();
	m_camera->setCameraComponent(shared_ptr<CameraComponent>(new CameraComponent(context->getScene().get())));
	m_camera->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));

	QString relativePath = path.mid(QDir::current().path().length() + 1);
	int meshId = context->getResources()->getIdForPath(relativePath.toStdString());
	context->getMeshCache()->getMesh(meshId)->onReady([this](auto mesh) { 
		m_mesh = mesh; 
		auto metaData = m_mesh->getMetaData();
		float scale = 0.8 * sqrt(pow(metaData->getXLength(), 2) + pow(metaData->getYLength(), 2) + pow(metaData->getZLength(), 2));
		glm::vec3 offset(0, 0.3 * scale, scale);
		m_camera->getTransformComponent()->setLookAt(metaData->getCenter() + offset, metaData->getCenter(), glm::vec3(0, 1, 0));
		if (m_initialized)
		{
			this->update();
		}
	});

	setFixedWidth(100);
	setFixedHeight(100);
}

void GLMeshWidget::initializeGL()
{
	shared_ptr<GraphicsInitOptions> options(new GraphicsInitOptions());
	options->setBufferDimensions(100, 100)->setShadowSize(1);
	m_graphicsContext->getGraphics()->init(options);
	m_initialized = true;
}

void GLMeshWidget::resizeGL(int width, int height)
{
	if (height > 0)
	{
		m_camera->getCameraComponent()->setViewport(glm::vec4(0, 0, width, height));
		m_camera->getCameraComponent()->setProjection(45.f, (float)width / (float)height, 0.01f, 100.f);
	}
}

void GLMeshWidget::paintGL()
{
	shared_ptr<RenderOptions> renderOptions(new RenderOptions());
	renderOptions->setCamera(m_camera->getCamera());
	renderOptions->setRenderTarget(RenderOptions::RenderTarget::SHADED);
	renderOptions->setDefaultFrameBufferId(defaultFramebufferObject());	

	shared_ptr<DirectLight> light(new DirectLight(glm::vec3(-0.5f, -1.f, -1.5f), glm::vec3(1), 50));
	renderOptions->addDirectLight(light);

	if (!m_renderData && m_mesh)
	{
		shared_ptr<VertexBufferHandle> vbo = m_graphicsContext->getGraphics()->createVertexBuffer(m_mesh);
		m_renderData = shared_ptr<RenderData>(new RenderData(vbo, DrawMode::TRIANGLES));
		m_renderData->setShadowsEnabled(false);
	}

	if (m_renderData)
	{
		m_graphicsContext->getGraphics()->clearDepthAndColor(defaultFramebufferObject());
		m_graphicsContext->getGraphics()->stageRender(m_renderData);
		m_graphicsContext->getGraphics()->executeRender(*renderOptions);
		m_graphicsContext->getGraphics()->drawScreen(*renderOptions);
	}
}