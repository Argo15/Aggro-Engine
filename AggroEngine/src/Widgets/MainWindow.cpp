#include "MainWindow.hpp"

#include <QtWidgets/QDockWidget>
#include <QPushButton>
#include <QAction>
#include <QMenuBar>
#include "SceneGraphTree.hpp"
#include "InspectorWidget.hpp"
#include "StringUtil.hpp"
#include "Config.hpp"

MainWindow::MainWindow()
	: m_context(shared_ptr<EngineContext>(new EngineContext()))
	, m_mainWidget(shared_ptr<MainWidget>(new MainWidget(m_context, this)))
{
	setCentralWidget(m_mainWidget.get());

	createMenus();

	const Properties& props = gConfig->getProperties();
	m_maxFps = props.getIntProperty("graphics.max_fps");
	startTimer(1, Qt::PreciseTimer);

	QDockWidget *leftWidget = new SceneGraphTree(m_context, this);
	leftWidget->setMinimumWidth(250);
	addDockWidget(Qt::LeftDockWidgetArea, leftWidget);

	QDockWidget *rightWidget = new QDockWidget();
	rightWidget->setMinimumWidth(350);
	InspectorWidget *inspectorWidget = new InspectorWidget(m_context, rightWidget);
	rightWidget->setWidget(inspectorWidget);
	rightWidget->setWindowTitle(QString::fromStdString("Inspector"));
	addDockWidget(Qt::RightDockWidgetArea, rightWidget);

}

void MainWindow::timerEvent(QTimerEvent *event)
{
	m_mainWidget->getGlWidget()->update();
}

/* createMenus() */
void MainWindow::createMenus()
{
	QAction *newAction = new QAction(tr("&New"), this);
	newAction->setShortcut(QKeySequence::New);
	connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

	QAction *openAction = new QAction(tr("&Open"), this);
	openAction->setShortcut(QKeySequence::Open);
	connect(openAction, &QAction::triggered, this, &MainWindow::open);

	QAction *saveAction = new QAction(tr("&Save"), this);
	openAction->setShortcut(QKeySequence::Save);
	connect(saveAction, &QAction::triggered, this, &MainWindow::save);

	QAction *exitAction = new QAction(tr("Exit"), this);
	openAction->setShortcut(QKeySequence::Close);
	connect(exitAction, &QAction::triggered, this, &MainWindow::close);

	QMenu *fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);

	QMenu *editMenu = menuBar()->addMenu(tr("Edit"));

	QMenu *toolsMenu = menuBar()->addMenu(tr("View"));

}

void MainWindow::newFile()
{
	
}

void MainWindow::open()
{
	
}

void MainWindow::save()
{
	
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	exit(0);
}