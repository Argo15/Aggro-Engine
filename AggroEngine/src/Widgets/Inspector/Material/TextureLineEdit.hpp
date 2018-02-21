#pragma once

#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <functional>
#include "MaterialComponent.hpp"
using namespace std;

class TextureLineEdit : public QLineEdit
{
private:
	shared_ptr<Resources> m_resources;
	MaterialComponent *m_material;
	function<void(MaterialComponent *, int)> m_setImageFunc;

protected:
	void dragMoveEvent(QDragMoveEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);

public:
	TextureLineEdit(shared_ptr<Resources> resources, function<void(MaterialComponent *, int)> setImageFunc);

	void setMaterial(MaterialComponent *material);
};