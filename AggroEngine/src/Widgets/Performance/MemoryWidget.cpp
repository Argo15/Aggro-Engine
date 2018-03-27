#include "MemoryWidget.hpp"
#include <QBoxLayout>
#include <QLabel>

MemoryWidget::MemoryWidget(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(new QLabel("Memory: "));
	layout->addStretch();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
}