#include "ModeSelector.hpp"

ModeSelector::ModeSelector(shared_ptr<RenderOptions> renderOptions, QWidget *parent)
	: m_renderOptions(renderOptions)
	, QComboBox(parent)
{
	addItem("Shaded");
	addItem("Albedo");
	addItem("Normal");
	addItem("Lighting");
	addItem("Selection");

	setFixedWidth(200);

	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(_modeChanged(int)));
}

void ModeSelector::_modeChanged(int index)
{
	m_renderOptions->setRenderTarget((RenderOptions::RenderTarget)index);
}