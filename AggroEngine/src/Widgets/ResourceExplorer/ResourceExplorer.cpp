#include "ResourceExplorer.hpp"
#include <QHBoxLayout>
#include <QScrollArea>
#include "GLResourceWidget.hpp"

ResourceExplorer::ResourceExplorer(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
	, m_resourceWidget(new GLResourceWidget())
{
	QHBoxLayout *layout = new QHBoxLayout;

	QScrollArea *scrollArea = new QScrollArea();
	scrollArea->setWidget(m_resourceWidget.get());
	scrollArea->setWidgetResizable(true);
	layout->addWidget(scrollArea);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
}

shared_ptr<GLResourceWidget> ResourceExplorer::getWidget()
{
	return m_resourceWidget;
}