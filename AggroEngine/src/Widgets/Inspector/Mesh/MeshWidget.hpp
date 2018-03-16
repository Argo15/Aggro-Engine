#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include "InspectorSubWidget.hpp"
#include "Locks.hpp"
#include "MeshCache.hpp"

class MeshWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QLineEdit> m_meshEdit;
	shared_ptr<QLabel> m_triangleCountLbl;
	shared_ptr<QComboBox> m_alignCombo;
	shared_ptr<QCheckBox> m_normalLineChk;
	shared_ptr<Resources> m_resources;
	shared_ptr<MeshCache> m_meshCache;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(MeshComponent *mesh);
	void _onMeshSelect();
	void _onNormalLineCheck(int state);

public:
	MeshWidget(QWidget *parent, shared_ptr<EngineContext> context);
};