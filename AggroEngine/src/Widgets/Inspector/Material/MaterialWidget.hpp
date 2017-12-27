#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "InspectorSubWidget.hpp"
#include "Locks.hpp"

class MaterialWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QLineEdit> m_colorREdit;
	shared_ptr<QLineEdit> m_colorGEdit;
	shared_ptr<QLineEdit> m_colorBEdit;
	shared_ptr<QLineEdit> m_textureEdit;
	shared_ptr<Resources> m_resources;
	boost::mutex m_textLock;
	glm::vec3 m_lastColor;
	int m_lastTextureId;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(MaterialComponent *transform);
	void _onColorChange(QString newValue);
	void _onTexSelect();

public:
	MaterialWidget(QWidget *parent, shared_ptr<Resources> resources);
};