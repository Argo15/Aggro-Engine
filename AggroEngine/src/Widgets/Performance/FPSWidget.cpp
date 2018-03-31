#include "FPSWidget.hpp"
#include <QBoxLayout>
#include <QLabel>
#include <QtCharts\QLineSeries>
#include <QtCharts\QChart>
using namespace QtCharts;

FPSWidget::FPSWidget(QWidget *parent)
	: QWidget(parent)
{
	/*QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(new QLabel("FPS: "));
	layout->addStretch();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QLineSeries *series = new QLineSeries();
	series->append(0, 6);
	series->append(2, 4);
	series->append(3, 8);
	series->append(7, 4);
	series->append(10, 5);
	*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

	QChart *chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(series);
	chart->createDefaultAxes();
	chart->setTitle("Simple line chart example");

	setLayout(layout);*/
}