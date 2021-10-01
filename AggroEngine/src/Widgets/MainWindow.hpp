#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>
#include "MainWidget.hpp"
#include "timer.hpp"
#include "EngineContext.hpp"
using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	shared_ptr<EngineContext> m_context;
	shared_ptr<MainWidget> m_mainWidget;
	shared_ptr<string> m_lastSaveFile;
	Timer timer;

	// action functions
	void newFile();
	void open();
	void save();
	void saveAs();

	// menus, actions, ect..
	void createMenus();

	shared_ptr<Scene> _loadDefaultScene();

protected:
	void timerEvent(QTimerEvent *event);
	void closeEvent(QCloseEvent *event);

public:
	MainWindow();
};