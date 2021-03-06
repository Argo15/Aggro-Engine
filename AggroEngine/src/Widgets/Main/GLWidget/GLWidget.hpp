#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include "FreeRoamCameraController.hpp"
#include "KeyboardState.hpp"
#include "MouseState.hpp"
#include "MouseController.hpp"
#include "Renderer.hpp"
#include "EngineContext.hpp"
#include "Clock.hpp"
#include "Selection.hpp"

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

private:
	shared_ptr<Renderer> m_renderer;
	shared_ptr<KeyboardState> m_keyboard;
	shared_ptr<MouseState> m_mouse;
	shared_ptr<MouseController> m_mouseController;
	shared_ptr<EngineContext> m_engineContext;
	shared_ptr<GraphicsContext> m_graphicsContext;
	shared_ptr<Clock> m_graphicsClock;
	shared_ptr<Selection> m_selection;
	shared_ptr<Job> m_cameraUpdateJob;
	int m_maxFps;
	int m_millisPerFrame;

	shared_ptr<Job> _setupCameraUpdateJob(shared_ptr<SceneNode> camera);
	void _dropTexture(QDropEvent *event);
	void _dropMesh(QDropEvent *event);
	shared_ptr<SceneNode> _getPreviewNode();

	void _refreshScene(Scene *scene);
	void _refreshCamera(shared_ptr<SceneNode> camera);

public:
	GLWidget(shared_ptr<EngineContext> context, QWidget *parent = 0);

	shared_ptr<GraphicsContext> getGraphicsContext();

protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);
};