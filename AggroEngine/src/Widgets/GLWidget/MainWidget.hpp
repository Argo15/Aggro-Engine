#pragma once

#include <QWidget>
#include "GLWidget.hpp"

class MainWidget : public QWidget
{
	Q_OBJECT

private:
	shared_ptr<EngineContext> m_context;
	shared_ptr<GLWidget> m_glWidget;

private slots:
	void _modeChanged(int index);

public:
	MainWidget(shared_ptr<EngineContext> context, QWidget *parent = 0);

	shared_ptr<GLWidget> getGlWidget();
};