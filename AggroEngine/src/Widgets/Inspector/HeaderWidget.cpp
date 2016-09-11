#include "HeaderWidget.hpp"
#include <QSizePolicy>

HeaderWidget::HeaderWidget(QWidget *parent)
	: InspectorSubWidget(parent)
	, m_nameEdit(shared_ptr<QLineEdit>(new QLineEdit("", this)))
{
	m_layout->addWidget(m_nameEdit.get());
	connect(m_nameEdit.get(), &QLineEdit::textChanged, this, &HeaderWidget::_onNameChange);
}

void HeaderWidget::_refresh(SceneNode *newNode)
{
	m_nameEdit->setText(QString::fromStdString(newNode->getName()));

	if (m_lastActiveNode && m_lastActiveNode.get() != newNode)
	{
		m_lastActiveNode->removeChangeListener(this);
	}

	if (!m_lastActiveNode || m_lastActiveNode.get() != newNode)
	{
		newNode->addChangeListener(this, [this](auto node) {this->_refresh(node);});
	}
}

void HeaderWidget::_onNameChange(QString newName)
{
	if (m_currentNode && newName.toStdString() != m_currentNode->getName())
	{
		m_currentNode->setName(newName.toStdString());
	}
}
