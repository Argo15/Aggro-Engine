#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "InspectorSubWidget.hpp"

class TransformWidget : public InspectorSubWidget
{
private:
	shared_ptr<QLineEdit> m_transXEdit;
	shared_ptr<QLineEdit> m_transYEdit;
	shared_ptr<QLineEdit> m_transZEdit;
	shared_ptr<QPushButton> m_resetRotate;
	shared_ptr<QLineEdit> m_scaleXEdit;
	shared_ptr<QLineEdit> m_scaleYEdit;
	shared_ptr<QLineEdit> m_scaleZEdit;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(TransformComponent *transform);
	void _onTransformChange(QString newValue);
	void _onRotateReset();

public:
	TransformWidget(QWidget *parent = nullptr);
};