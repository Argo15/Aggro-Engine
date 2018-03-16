#include "InspectorWidget.hpp"
#include "HeaderWidget.hpp"
#include "TransformWidget.hpp"
#include "DirectLightWidget.hpp"
#include "MaterialWidget.hpp"
#include "AddMaterialWidget.hpp"
#include "DelegateMaterialWidget.hpp"
#include "MeshWidget.hpp"
#include "CameraWidget.hpp"
#include <QLabel>

InspectorWidget::InspectorWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
	, m_context(context)
{
	m_layout = new QVBoxLayout();
	shared_ptr<InspectorSubWidget> transformWidget(new TransformWidget(this, context));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new HeaderWidget(this)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(transformWidget));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new MeshWidget(this, context)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new DirectLightWidget(this)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new MaterialWidget(this, context)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new AddMaterialWidget(this)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new DelegateMaterialWidget(this, context)));
	m_widgets.push_back(shared_ptr<InspectorSubWidget>(new CameraWidget(this, context)));
	for (auto widget : m_widgets)
	{
		m_layout->addWidget(widget.get());
	}
	m_layout->setContentsMargins(0, 7, 0, 0);
	m_layout->addStretch();

	refresh(m_context->getScene()->getSelectedNode());
	m_context->addNewSceneListener([this, transformWidget](auto scene) {
		_addListeners(scene, transformWidget);
	});
	_addListeners(m_context->getScene().get(), transformWidget);


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

void InspectorWidget::_addListeners(Scene *scene, shared_ptr<InspectorSubWidget> transformWidget)
{
	scene->addSelectionChangeListener([this](auto node) {refresh(node); });
	scene->addCameraChangeListener([transformWidget](auto camera) {
		if (transformWidget->getCurrentNode() == camera)
		{
			transformWidget->hide();
		}
	});
}

