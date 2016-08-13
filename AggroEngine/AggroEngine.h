#ifndef AGGROENGINE_H
#define AGGROENGINE_H

#include <QtWidgets/QMainWindow>
#include "ui_AggroEngine.h"

class AggroEngine : public QMainWindow
{
	Q_OBJECT

public:
	AggroEngine(QWidget *parent = 0);
	~AggroEngine();

private:
	Ui::AggroEngineClass ui;
};

#endif // AGGROENGINE_H
