#pragma once

#include "EngineContext.hpp"
#include "InspectorSubWidget.hpp"
#include <QDockWidget>
#include <memory>
#include <QVBoxLayout>
using namespace std;

/**
* Shows details about the selected scene node
*
* author: wcrane
* since: 2016-08-28
**/
class InspectorWidget : public QWidget
{
private:
	shared_ptr<EngineContext> m_context;
	vector<shared_ptr<InspectorSubWidget>> m_widgets;
	QWidget *m_widget;
	QVBoxLayout *m_layout;

public:
	InspectorWidget(shared_ptr<EngineContext> context, QWidget *parent = nullptr);

	void refresh(shared_ptr<SceneNode> node);
};