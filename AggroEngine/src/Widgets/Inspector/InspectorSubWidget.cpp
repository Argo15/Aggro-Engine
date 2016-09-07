#include "InspectorSubWidget.hpp" 
#include <QStyle>

InspectorSubWidget::InspectorSubWidget(QWidget *parent)
	: QWidget(parent)
	, m_layout(shared_ptr<QVBoxLayout>(new QVBoxLayout()))
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	this->setStyleSheet(QString("border-bottom-style: outset; border-width: 2px; border-color: #BBB;"));
	m_layout->setContentsMargins(5, 0, 8, 10);
	setLayout(m_layout.get());
}

void InspectorSubWidget::refresh(shared_ptr<SceneNode> node)
{
	if (node)
	{
		show();
		_refresh(node);
	}
	else
	{
		hide();
	}
}