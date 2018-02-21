#include "TextureLineEdit.hpp"
#include <memory>
#include <QMimeData>
#include <iostream>

TextureLineEdit::TextureLineEdit(shared_ptr<Resources> resources,
		function<void(MaterialComponent *, int)> setImageFunc)
	: QLineEdit("")
	, m_resources(resources)
	, m_setImageFunc(setImageFunc)
{
	setAcceptDrops(true);
}

void TextureLineEdit::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-texture"))
	{
		event->acceptProposedAction();
	}
}

void TextureLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-texture"))
	{
		event->acceptProposedAction();
	}
}

void TextureLineEdit::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void TextureLineEdit::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-texture"))
	{
		QString filepath = QString::fromLocal8Bit(event->mimeData()->data("application/x-texture"));
		setText(filepath);
		int id = m_resources->getIdForPath(filepath.toStdString());
		m_setImageFunc(m_material, id);
	}
	event->accept();
}

void TextureLineEdit::setMaterial(MaterialComponent *material)
{
	m_material = material;
}