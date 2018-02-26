#pragma once

#include <QListWidget>
#include "GLResourceWidget.hpp"

class QuickResources : public QListWidget
{
private:

public:
	QuickResources(GLResourceWidget *resourceWidget);
};