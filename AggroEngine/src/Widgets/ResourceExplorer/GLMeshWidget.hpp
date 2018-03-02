#pragma once

#include "EngineContext.hpp"
#include "GraphicsContext.hpp"
#include "Camera.hpp"
#include "RenderData.hpp"
#include <QGLWidget>
#include <QWidget>

class GLMeshWidget : public QGLWidget
{
private:
	shared_ptr<EngineContext> m_engineContext;
	shared_ptr<GraphicsContext> m_graphicsContext;
	shared_ptr<Mesh> m_mesh;
	shared_ptr<Camera> m_camera;
	shared_ptr<RenderData> m_renderData;

	bool m_initialized;

public:
	GLMeshWidget(QString &path, shared_ptr<EngineContext> context, QWidget *parent);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
};