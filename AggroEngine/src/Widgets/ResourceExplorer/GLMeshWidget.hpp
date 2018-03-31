#pragma once

#define GLEW_STATIC
#include "EngineContext.hpp"
#include "GraphicsContext.hpp"
#include "Camera.hpp"
#include "RenderData.hpp"
#include <QOpenGLWidget>
#include <QWidget>

class GLMeshWidget : public QOpenGLWidget
{
private:
	shared_ptr<EngineContext> m_engineContext;
	shared_ptr<GraphicsContext> m_graphicsContext;
	shared_ptr<Mesh> m_mesh;
	shared_ptr<SceneNode> m_camera;
	shared_ptr<RenderData> m_renderData;

	bool m_initialized;

public:
	GLMeshWidget(QString &path, shared_ptr<EngineContext> context, QWidget *parent = 0);

protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
};