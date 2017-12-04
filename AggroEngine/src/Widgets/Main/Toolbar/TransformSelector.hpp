#pragma once

#include <QComboBox>
#include <QWidget>
#include <QPushButton>
#include <memory>
#include "Scene.hpp"
#include "TranslateHook.hpp"
#include "RotateHook.hpp"
#include "ScaleHook.hpp"
using namespace std;

class TransformSelector : public QWidget
{
	Q_OBJECT

private:
	shared_ptr<EngineContext> m_context;
	shared_ptr<TransformHook> m_currentHook;
	shared_ptr<TranslateHook> m_translateHook;
	shared_ptr<RotateHook> m_rotateHook;
	shared_ptr<ScaleHook> m_scaleHook;
	shared_ptr<QPushButton> m_transBtn, m_rotBtn, m_scaleBtn;

	void _select(shared_ptr<QPushButton> button, shared_ptr<TransformHook> hook);

public:
	TransformSelector(shared_ptr<EngineContext> context, QWidget *parent = nullptr);
};