#include "MainWindow.hpp"

#include <QtWidgets/QDockWidget>
#include <QPushButton>
#include <QAction>
#include <QMenuBar>
#include <QDir>
#include <QFileDialog>
#include <QScrollArea>
#include "SceneGraphTree.hpp"
#include "InspectorWidget.hpp"
#include "StringUtil.hpp"
#include "Config.hpp"
#include "FileWriter.hpp"
#include "ResourceExplorer.hpp"
#include "QuickResources.hpp"

MainWindow::MainWindow()
	: m_context(shared_ptr<EngineContext>(new EngineContext()))
	, m_mainWidget(shared_ptr<MainWidget>(new MainWidget(m_context, this)))
	, m_lastSaveFile()
{
	m_context->setScene(_loadDefaultScene());
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

	QDockWidget *bottomWidget = new QDockWidget();
	bottomWidget->setMinimumHeight(365);
	ResourceExplorer *resourceWidget = new ResourceExplorer(m_context, bottomWidget);
	bottomWidget->setWidget(resourceWidget);
	bottomWidget->setWindowTitle(QString::fromStdString("Resources"));

	QDockWidget *bottomLeftWidget = new QDockWidget();
	QWidget *quickResourcesWidget = new QuickResources(resourceWidget->getWidget().get());
	quickResourcesWidget->setFixedWidth(255);
	bottomLeftWidget->setWidget(quickResourcesWidget);
	bottomLeftWidget->setWindowTitle(QString::fromStdString("Quick Resources"));
	addDockWidget(Qt::BottomDockWidgetArea, bottomLeftWidget);
	addDockWidget(Qt::BottomDockWidgetArea, bottomWidget);
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
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, &QAction::triggered, this, &MainWindow::save);

	QAction *saveAsAction = new QAction(tr("&Save As"), this);
	connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

	QAction *exitAction = new QAction(tr("Exit"), this);
	exitAction->setShortcut(QKeySequence::Close);
	connect(exitAction, &QAction::triggered, this, &MainWindow::close);

	QMenu *fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addAction(exitAction);

	QMenu *editMenu = menuBar()->addMenu(tr("Edit"));

	QMenu *toolsMenu = menuBar()->addMenu(tr("View"));
}

void MainWindow::newFile()
{
	m_lastSaveFile = shared_ptr<string>();
	shared_ptr<Scene> scene(new Scene(
		shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()))
	));
	m_context->setScene(scene);
}

void MainWindow::open()
{
	QDir workingDirectory = QDir::current();
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Scene"), workingDirectory.path() + "/Saves/", "*.ascn");
	m_lastSaveFile = shared_ptr<string>(new string(filename.toStdString()));
	if (shared_ptr<Chunk> chunk = FileWriter::readFile(filename.toStdString()))
	{
		shared_ptr<Scene> scene = Scene::deserialize(chunk.get(), m_context);
		if (scene)
		{
			m_context->setScene(scene);
		}
	}
}

void MainWindow::save()
{
	if (!m_lastSaveFile)
	{
		saveAs();
	}
	else
	{
		shared_ptr<Chunk> chunk = m_context->getScene()->serialize(m_context->getResources());
		FileWriter::writeToFile(*m_lastSaveFile.get(), chunk);
	}
}

void MainWindow::saveAs()
{
	QDir workingDirectory = QDir::current();
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Scene"), workingDirectory.path() + "/Saves/", "*.ascn");
	shared_ptr<StaticObjectRenderComponent> renderComponent(new StaticObjectRenderComponent());
	shared_ptr<Chunk> chunk = m_context->getScene()->serialize(m_context->getResources());
	FileWriter::writeToFile(filename.toStdString(), chunk);
	m_lastSaveFile = shared_ptr<string>(new string(filename.toStdString()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	exit(0);
}

shared_ptr<Scene> MainWindow::_loadDefaultScene()
{
	QDir workingDirectory = QDir::current();
	if (shared_ptr<Chunk> chunk = FileWriter::readFile(workingDirectory.path().toStdString() + "/Saves/default.ascn"))
	{
		return Scene::deserialize(chunk.get(), m_context);
	}
	return shared_ptr<Scene>(new Scene());
}