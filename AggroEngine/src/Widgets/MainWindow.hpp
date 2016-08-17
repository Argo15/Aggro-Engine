#pragma once

#include "GLWidget.hpp"
#include "../Util/timer.hpp"
#include <QtWidgets/QMainWindow>
#include <memory>
using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	shared_ptr<GLWidget> m_glWidget;
	Timer timer;

public:
	MainWindow();

protected:
	void timerEvent(QTimerEvent *event);
};