#include "TransformSelector.hpp"
#include <QHBoxLayout>

TransformSelector::TransformSelector(shared_ptr<Scene> scene, QWidget *parent)
	: QWidget(parent)
	, m_scene(scene)
	, m_translateHook(new TranslateHook())
	, m_scaleHook(new ScaleHook())
{
	m_transBtn = shared_ptr<QPushButton>(new QPushButton("T"));
	m_rotBtn = shared_ptr<QPushButton>(new QPushButton("R"));
	m_scaleBtn = shared_ptr<QPushButton>(new QPushButton("S"));

	QHBoxLayout *layout = new QHBoxLayout;

	layout->addWidget(m_transBtn.get());
	layout->addWidget(m_rotBtn.get());
	layout->addWidget(m_scaleBtn.get());
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	int size = 40;
	m_transBtn->setFixedSize(size, size);
	m_rotBtn->setFixedSize(size, size);
	m_scaleBtn->setFixedSize(size, size);

	//m_transBtn->setStyleSheet("background-color: #BBB; border: 1px solid #888;");
	setStyleSheet("font-weight: bold;");

	connect(m_transBtn.get(), &QPushButton::clicked, this, [this](auto pressed) { _select(m_transBtn, m_translateHook); });
	connect(m_rotBtn.get(), &QPushButton::clicked, this, [this](auto pressed) { _select(m_rotBtn, m_translateHook); });
	connect(m_scaleBtn.get(), &QPushButton::clicked, this, [this](auto pressed) { _select(m_scaleBtn, m_scaleHook); });

	setLayout(layout);

	_select(m_transBtn, m_translateHook);
}

void TransformSelector::_select(shared_ptr<QPushButton> button, shared_ptr<TransformHook> hook)
{
	m_transBtn->setStyleSheet("");
	m_rotBtn->setStyleSheet("");
	m_scaleBtn->setStyleSheet("");

	button->setStyleSheet("background-color: #BBB; border: 1px solid #888;");
	m_scene->setTransformHook(hook);
}