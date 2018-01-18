#include "InspectorWidget.hpp"
#include "HeaderWidget.hpp"
#include "TransformWidget.hpp"
#include "DirectLightWidget.hpp"
#include "MaterialWidget.hpp"
#include "AddMaterialWidget.hpp"
#include "DelegateMaterialWidget.hpp"
#include "MeshWidget.hpp"
#include <QLabel>

InspectorWidget::InspectorWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
	, m_context(context)
{
	m_layout = new QVBoxLayout();
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new HeaderWidget(this)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new TransformWidget(this)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new MeshWidget(this, context->getResources(), context->getMeshImporter())));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new DirectLightWidget(this)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new MaterialWidget(this, context->getResources())));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new AddMaterialWidget(this)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new DelegateMaterialWidget(this, context->getScene())));
	for (auto widget : m_widgets)
	{
		m_layout->addWidget(widget.get());
	}
	m_layout->setContentsMargins(0, 7, 0, 0);
	m_layout->addStretch();

	refresh(m_context->getScene()->getSelectedNode());
	m_context->getScene()->addSelectionChangeListener([this](auto node) {refresh(node);});
	m_context->addNewSceneListener([this](auto scene) {
		scene->addSelectionChangeListener([this](auto node) {refresh(node); });
	});

	setLayout(m_layout);
	this->show();
}

void InspectorWidget::refresh(shared_ptr<SceneNode> node)
{
	for (auto widget : m_widgets)
	{
		widget->refresh(node);
	}
}
