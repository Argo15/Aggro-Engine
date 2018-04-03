#pragma once

#include <QtCharts\QLineSeries>
#include <QtCharts\QChart>
#include <QtCharts\QChartView>
#include <QWidget>
#include <QDockWidget>
#include <memory>
#include <deque>

class FPSWidget : public QWidget
{
private:
	QDockWidget *m_parent;
	std::shared_ptr<QtCharts::QLineSeries> m_fpsMinLines;
	QtCharts::QChart *m_chart;
	QtCharts::QChartView *m_chartView;
	int m_xAxisSize;

	void _onUpdate(std::deque<int> *fpsTicks);

public:
	FPSWidget(QDockWidget *parent = nullptr);
};