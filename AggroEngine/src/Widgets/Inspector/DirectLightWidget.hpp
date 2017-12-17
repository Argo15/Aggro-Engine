#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "InspectorSubWidget.hpp"
#include "Locks.hpp"

class DirectLightWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QSlider> m_ambientSlider;
	shared_ptr<QLineEdit> m_colorREdit;
	shared_ptr<QLineEdit> m_colorGEdit;
	shared_ptr<QLineEdit> m_colorBEdit;
	boost::mutex m_textLock;
	glm::vec3 m_lastColor;
	int m_lastAmbient;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(DirectLightComponent *transform);
	void _onColorChange(QString newValue);
	void _onAmbientChange(int value);

public:
	DirectLightWidget(QWidget *parent = nullptr);
};