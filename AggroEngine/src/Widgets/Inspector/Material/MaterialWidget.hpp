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
	shared_ptr<QLineEdit> m_alphaEdit;
	shared_ptr<Resources> m_resources;
	boost::mutex m_textLock;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(MaterialComponent *transform);
	void _onColorChange(QString newValue);
	void _onTexSelect(shared_ptr<QLineEdit> textureEdit, 
					  function<void(shared_ptr<MaterialComponent>, int)> setImageFunc,
					  function<void(shared_ptr<MaterialComponent>)> removeFunc);

public:
	MaterialWidget(QWidget *parent, shared_ptr<Resources> resources);
};