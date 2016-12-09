#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#include <QKeyEvent>
#include "FreeRoamCameraController.hpp"
#include "KeyboardState.hpp"
#include "MouseState.hpp"
#include "MouseController.hpp"
#include "Renderer.hpp"
#include "EngineContext.hpp"
#include "Clock.hpp"
#include "Selection.hpp"

class GLWidget : public QGLWidget
{
	Q_OBJECT

private:
	shared_ptr<Renderer> m_renderer;
	shared_ptr<KeyboardState> m_keyboard;
	shared_ptr<MouseState> m_mouse;
	shared_ptr<MouseController> m_mouseController;
	shared_ptr<EngineContext> m_context;
	shared_ptr<Clock> m_graphicsClock;
	shared_ptr<Selection> m_selection;
	int m_maxFps;

public:
	GLWidget(shared_ptr<EngineContext> context, QWidget *parent = 0);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
};