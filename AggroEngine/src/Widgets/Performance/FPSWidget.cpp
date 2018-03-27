#include "FPSWidget.hpp"
#include <QBoxLayout>
#include <QLabel>

FPSWidget::FPSWidget(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(new QLabel("FPS: "));
	layout->addStretch();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
}