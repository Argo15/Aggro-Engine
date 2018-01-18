#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include "InspectorSubWidget.hpp"
#include "Locks.hpp"

class MeshWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QLineEdit> m_meshEdit;
	shared_ptr<QLabel> m_triangleCountLbl;
	shared_ptr<Resources> m_resources;
	shared_ptr<MeshImporter> m_importer;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(MeshComponent *mesh);
	void _onMeshSelect();

public:
	MeshWidget(QWidget *parent, shared_ptr<Resources> resources, shared_ptr<MeshImporter> importer);
};