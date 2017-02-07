#include "MainWidget.hpp"
#include "Toolbar.hpp"
#include <QBoxLayout>
#include <QComboBox>

MainWidget::MainWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
	, m_context(context)
	, m_glWidget(shared_ptr<GLWidget>(new GLWidget(m_context, this)))
{
	QVBoxLayout *layout = new QVBoxLayout;

	layout->addWidget(new Toolbar(m_context));
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