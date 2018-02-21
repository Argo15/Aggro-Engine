#pragma once

#include <QWidget>
#include "EngineContext.hpp"

class ResourceExplorer : public QWidget
{
	Q_OBJECT

private:
	shared_ptr<EngineContext> m_context;

public:
	ResourceExplorer(shared_ptr<EngineContext> context, QWidget *parent = 0);
};