#include "DelegateMaterialWidget.hpp"

DelegateMaterialWidget::DelegateMaterialWidget(InspectorWidget *parent, shared_ptr<Scene> scene)
	: InspectorSubWidget(parent)
	, m_editBaseBtn(new QPushButton("Edit Base Material"))
	, m_copyBaseBtn(new QPushButton("Copy Base Material"))
{
	QHBoxLayout *mainLayout = new QHBoxLayout;

	QLabel *lbl;

	mainLayout->addWidget(m_editBaseBtn.get());
	mainLayout->addWidget(m_copyBaseBtn.get());

	lbl = new QLabel("Material");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(mainLayout);

	connect(m_editBaseBtn.get(), &QPushButton::pressed, this, [this, parent, scene]() {
		if (m_currentNode)
		{
			scene->selectNodeById(m_currentNode->getMaterialComponent()->getOwner()->getId());
			parent->refresh(scene->getSelectedNode());
		}
	});

	connect(m_copyBaseBtn.get(), &QPushButton::pressed, this, [this, parent]() {
		if (m_currentNode)
		{
			shared_ptr<MaterialComponent> newMat(new MaterialComponent(m_currentNode.get(), m_currentNode->getMaterialComponent()));
			m_currentNode->setMaterialComponent(newMat);
			parent->refresh(m_currentNode);
		}
	});
}

void DelegateMaterialWidget::_refresh(SceneNode *newNode)
{
	boost::lock_guard<DelegateMaterialWidget> guard(*this);
	if (!newNode->hasMaterialComponent() || 
		newNode == newNode->getMaterialComponent()->getOwner())
	{
		this->hide();
		return;
	}
}