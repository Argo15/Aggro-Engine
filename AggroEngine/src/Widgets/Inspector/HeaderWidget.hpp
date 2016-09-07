#pragma once

#include <QLineEdit>
#include "InspectorSubWidget.hpp"

class HeaderWidget : public InspectorSubWidget
{
private:
	shared_ptr<QLineEdit> m_nameEdit;

protected:
	virtual void _refresh(shared_ptr<SceneNode> node);

public:
	HeaderWidget(QWidget *parent = nullptr);
};