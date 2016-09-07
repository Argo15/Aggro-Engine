#include "MainWindow.hpp"

#include <QtWidgets/QDockWidget>
#include <QPushButton>
#include <QAction>
#include <QMenuBar>
#include "SceneGraphTree.hpp"
#include "InspectorWidget.hpp"
#include "StringUtil.hpp"

MainWindow::MainWindow()
	: m_context(shared_ptr<EngineContext>(new EngineContext()))
	, m_glWidget(shared_ptr<GLWidget>(new GLWidget(m_context, this)))
{
	setCentralWidget(m_glWidget.get());

	createMenus();

	startTimer(0);
	timer.Init();

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
	static float time = 0;
	time += 1.0 / timer.GetFPS();
	if (time >= (1.0 / 60))
	{
		m_context->setFPS(60);
		m_glWidget->repaint();
		time = 0;
	}
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