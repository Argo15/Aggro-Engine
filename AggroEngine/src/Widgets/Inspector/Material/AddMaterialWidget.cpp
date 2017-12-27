#include "AddMaterialWidget.hpp"

AddMaterialWidget::AddMaterialWidget(InspectorWidget *parent)
	: InspectorSubWidget(parent)
	, m_addMaterialBtn(new QPushButton("Add Material"))
{
	QHBoxLayout *mainLayout = new QHBoxLayout;

	m_addMaterialBtn->setFixedWidth(300);

	QLabel *lbl;

	mainLayout->addWidget(m_addMaterialBtn.get());

	lbl = new QLabel("Material");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(mainLayout);

	connect(m_addMaterialBtn.get(), &QPushButton::pressed, this, [this, parent]() {
		if (m_lastActiveNode)
		{
			m_lastActiveNode->setMaterialComponent(shared_ptr<MaterialComponent>(new MaterialComponent()));
			parent->refresh(m_lastActiveNode);
		}
	});
}

void AddMaterialWidget::_refresh(SceneNode *newNode)
{
	boost::lock_guard<AddMaterialWidget> guard(*this);
	if (newNode->hasMaterialComponent() || newNode->hasDirectLightComponent())
	{
		this->hide();
		return;
	}
}