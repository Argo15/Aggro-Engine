#include "Toolbar.hpp"
#include "ModeSelector.hpp"
#include "TransformSelector.hpp"
#include <QHBoxLayout>

Toolbar::Toolbar(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *toolbarLayout = new QHBoxLayout;

	toolbarLayout->addWidget(new ModeSelector(context->getRenderOptions()));
	toolbarLayout->addWidget(new TransformSelector(context->getScene()));
	toolbarLayout->addStretch();

	toolbarLayout->setContentsMargins(5, 0, 0, 0);

	setFixedHeight(50);
	setLayout(toolbarLayout);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}