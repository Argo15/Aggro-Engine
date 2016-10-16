#include "MainWidget.hpp"
#include <QHBoxLayout>
#include <QComboBox>

MainWidget::MainWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
	, m_context(context)
	, m_glWidget(shared_ptr<GLWidget>(new GLWidget(m_context, this)))
{
	QVBoxLayout *layout = new QVBoxLayout;
		QHBoxLayout *toolbarLayout = new QHBoxLayout;

	QComboBox *renderMode = new QComboBox();
	renderMode->addItem("Shaded");
	renderMode->addItem("Albedo");
	renderMode->addItem("Normal");
	renderMode->addItem("Selection");
	renderMode->setFixedWidth(200);
	connect(renderMode, SIGNAL(currentIndexChanged(int)), SLOT(_modeChanged(int)));
	toolbarLayout->addWidget(renderMode);
	toolbarLayout->addStretch();

	layout->addLayout(toolbarLayout);
	layout->addWidget(m_glWidget.get());
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
}

shared_ptr<GLWidget> MainWidget::getGlWidget()
{
	return m_glWidget;
}

void MainWidget::_modeChanged(int index)
{
	m_context->getRenderOptions()->setRenderTarget((RenderOptions::RenderTarget)index);
}