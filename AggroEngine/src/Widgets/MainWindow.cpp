#include "MainWindow.hpp"

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>

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
	: m_glWidget(shared_ptr<GLWidget>(new GLWidget(this)))
{
	setCentralWidget(m_glWidget.get());

	startTimer(0);
	timer.Init();

	// Test
	QDockWidget *topWidget = new DockedActorEditor("Test Top", this);
	QDockWidget *leftWidget = new DockedActorEditor("Test Left", this);
	QDockWidget *rightWidget = new DockedActorEditor("Test Right", this);
	QDockWidget *rightWidget2 = new DockedActorEditor("Test Right 2", this);
	addDockWidget(Qt::TopDockWidgetArea, topWidget);
	addDockWidget(Qt::LeftDockWidgetArea, leftWidget);
	addDockWidget(Qt::RightDockWidgetArea, rightWidget);
	addDockWidget(Qt::RightDockWidgetArea, rightWidget2);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	//m_glWidget->setFPS(timer.GetFPS());
	m_glWidget->setFPS(60); // seems to be more accurate
	m_glWidget->repaint();
}