#include "HeaderWidget.hpp"
#include <QSizePolicy>

HeaderWidget::HeaderWidget(QWidget *parent)
	: InspectorSubWidget(parent)
	, m_nameEdit(shared_ptr<QLineEdit>(new QLineEdit("", this)))
{
	m_layout->addWidget(m_nameEdit.get());
}

void HeaderWidget::_refresh(shared_ptr<SceneNode>node)
{
	m_nameEdit->setText(QString::fromStdString(node->getName()));
}