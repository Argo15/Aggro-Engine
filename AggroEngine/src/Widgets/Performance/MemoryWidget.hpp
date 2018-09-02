#pragma once

#include <QWidget>
#include <memory>
#include <QtWidgets/QDockWidget>
#include <boost/unordered_map.hpp>
#include <QtCharts\QChart>
#include <QtCharts\QBarSet>
#include <QtCharts\QBarCategoryAxis>
#include <QtCharts\QHorizontalStackedBarSeries>
using namespace std;

class MemoryWidget : public QWidget
{
private:
	shared_ptr<QtCharts::QChart> m_chart;
	shared_ptr<QtCharts::QBarSet> m_barSet;
	shared_ptr<QtCharts::QHorizontalStackedBarSeries> m_barSeries;
	shared_ptr<QtCharts::QBarCategoryAxis> m_axis;

	void _onUpdate(boost::unordered_map<string, long> &nameToBytesAllocated);

public:
	MemoryWidget(QDockWidget *parent = nullptr);
};