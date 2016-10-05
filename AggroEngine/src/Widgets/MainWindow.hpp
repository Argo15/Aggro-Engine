#pragma once

#include "MainWidget.hpp"
#include "timer.hpp"
#include "EngineContext.hpp"
#include <QtWidgets/QMainWindow>
#include <memory>
using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	shared_ptr<EngineContext> m_context;
	shared_ptr<MainWidget> m_mainWidget;
	Timer timer;

	// action functions
	void newFile();
	void open();
	void save();

	// menus, actions, ect..
	void createMenus();

protected:
	void timerEvent(QTimerEvent *event);
	void closeEvent(QCloseEvent *event);

public:
	MainWindow();
};