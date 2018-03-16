#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "InspectorSubWidget.hpp"
#include "Locks.hpp"

class TransformWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QLineEdit> m_transXEdit;
	shared_ptr<QLineEdit> m_transYEdit;
	shared_ptr<QLineEdit> m_transZEdit;
	shared_ptr<QLineEdit> m_rotXEdit;
	shared_ptr<QLineEdit> m_rotYEdit;
	shared_ptr<QLineEdit> m_rotZEdit;
	shared_ptr<QPushButton> m_resetRotate;
	shared_ptr<QLineEdit> m_scaleXEdit;
	shared_ptr<QLineEdit> m_scaleYEdit;
	shared_ptr<QLineEdit> m_scaleZEdit;
	boost::mutex m_textLock;
	glm::vec3 m_lastTranslate;
	glm::vec3 m_lastRotate;
	glm::vec3 m_lastScale;
	shared_ptr<EngineContext> m_context;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(TransformComponent *transform);
	void _onTransformChange(QString newValue);

public:
	TransformWidget(QWidget *parent, shared_ptr<EngineContext> context);
};