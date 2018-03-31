#pragma once

#include <QWidget>
#include "EngineContext.hpp"
#include "GLResourceWidget.hpp"

class ResourceExplorer : public QWidget
{
	Q_OBJECT

private:
	shared_ptr<GLResourceWidget> m_resourceWidget;

public:
	ResourceExplorer(shared_ptr<EngineContext> context, QWidget *parent = nullptr);

	shared_ptr<GLResourceWidget> getWidget();
};