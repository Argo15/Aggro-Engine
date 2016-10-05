#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#include <QKeyEvent>
#include "../Core/Object/Cameras/FreeRoamCameraController.hpp"
#include "../Inputs/KeyboardState.hpp"
#include "../Inputs/MouseState.hpp"
#include "Renderer.hpp"
#include "EngineContext.hpp"

class GLWidget : public QGLWidget
{
	Q_OBJECT

private:
	shared_ptr<Renderer> m_renderer;

	shared_ptr<KeyboardState> m_keyboard;
	shared_ptr<MouseState> m_mouse;
	shared_ptr<CameraController> m_cameraController;

	shared_ptr<EngineContext> m_context;

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