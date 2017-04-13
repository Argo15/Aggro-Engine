#pragma once

#include <memory>
#include <QWidget>
#include "EngineContext.hpp"
using namespace std;

class Toolbar : public QWidget
{
	Q_OBJECT

public:
	Toolbar(shared_ptr<EngineContext> context, QWidget *parent = nullptr);
};