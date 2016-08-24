#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#include <QKeyEvent>
#include "../Graphics/OpenGL/OpenGL43Graphics.hpp"
#include "../Core/Object/Cameras/FreeRoamCameraController.hpp"
#include "../Inputs/KeyboardState.hpp"
#include "../Inputs/MouseState.hpp"
#include "../Resource/Mesh/Dev/Grid.hpp"
#include "../Core/Scene/Scene.hpp"
#include "VertexBufferCache.hpp"
#include "TextureCache.hpp"
#include "Renderer.hpp"

class GLWidget : public QGLWidget
{
	Q_OBJECT

private:
	shared_ptr<Graphics> m_graphics;
	shared_ptr<Renderer> m_renderer;

	shared_ptr<KeyboardState> m_keyboard;
	shared_ptr<MouseState> m_mouse;
	shared_ptr<CameraController> m_cameraController;
	int m_nFPS;

	shared_ptr<VertexBufferCache> m_vboCache;
	shared_ptr<TextureCache> m_textureCache;

	shared_ptr<Scene> m_scene;

public:
	GLWidget(QWidget *parent = 0);

	void setFPS(int fps);

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