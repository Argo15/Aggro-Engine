#include "ProfilingWidget.hpp"
#include <QBoxLayout>
#include <QLabel>

ProfilingWidget::ProfilingWidget(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(new QLabel("Profiling: "));
	layout->addStretch();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
}