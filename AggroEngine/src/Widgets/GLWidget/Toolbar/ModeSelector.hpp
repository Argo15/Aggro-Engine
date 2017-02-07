#pragma once

#include <QComboBox>
#include <QWidget>
#include <memory>
#include "RenderOptions.hpp"
using namespace std;

class ModeSelector : public QComboBox
{
	Q_OBJECT

private:
	shared_ptr<RenderOptions> m_renderOptions;

private slots:
	void _modeChanged(int index);

public:
	ModeSelector(shared_ptr<RenderOptions> renderOptions, QWidget *parent = nullptr);
};