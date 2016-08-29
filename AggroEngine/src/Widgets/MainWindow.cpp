#include "MainWindow.hpp"

#include <QtWidgets/QDockWidget>
#include <QPushButton>
#include <QAction>
#include <QMenuBar>
#include "SceneGraphTree.hpp"

class DockedActorEditor : public QDockWidget
{
public:
	DockedActorEditor(string name, QWidget *parent)
	{
		setWindowTitle(name.c_str());
		setWidget(new QPushButton("Test", parent));
		setAutoFillBackground(true);
		setBackgroundRole(QPalette::Dark);
	}
};

MainWindow::MainWindow()
	: m_context(shared_ptr<EngineContext>(new EngineContext()))
	, m_glWidget(shared_ptr<GLWidget>(new GLWidget(m_context, this)))
{
	setCentralWidget(m_glWidget.get());

	createMenus();

	startTimer(0);
	timer.Init();

	// Test
	QDockWidget *leftWidget = new SceneGraphTree(m_context, this);
	addDockWidget(Qt::LeftDockWidgetArea, leftWidget);

	/*QDockWidget *topWidget = new DockedActorEditor("Test Top", this);
	QDockWidget *rightWidget = new DockedActorEditor("Test Right", this);
	QDockWidget *rightWidget2 = new DockedActorEditor("Test Right 2", this);
	addDockWidget(Qt::BottomDockWidgetArea, topWidget);
	addDockWidget(Qt::RightDockWidgetArea, rightWidget);
	addDockWidget(Qt::RightDockWidgetArea, rightWidget2);*/
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	//m_glWidget->setFPS(timer.GetFPS());
	m_context->setFPS(60); // seems to be more accurate
	m_glWidget->repaint();
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