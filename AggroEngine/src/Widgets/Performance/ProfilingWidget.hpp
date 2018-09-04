#pragma once

#include <QWidget>
#include <memory>
#include <QtWidgets/QDockWidget>
#include <boost/unordered_map.hpp>
#include <QtCharts\QChart>
#include <QtCharts\QBarSet>
#include <QtCharts\QBarCategoryAxis>
#include <QtCharts\QHorizontalBarSeries>
#include "ProfilingData.hpp"
using namespace std;

class ProfilingWidget : public QWidget
{
private:
	QDockWidget *m_parent;
	shared_ptr<QtCharts::QChart> m_chart;
	shared_ptr<QtCharts::QBarSet> m_maxBarSet;
	shared_ptr<QtCharts::QBarSet> m_avgBarSet;
	shared_ptr<QtCharts::QHorizontalBarSeries> m_barSeries;
	shared_ptr<QtCharts::QBarCategoryAxis> m_axis;

	void _onUpdate(boost::unordered_map<string, shared_ptr<ProfilingData>> &nameToProfilingData);

public:
	ProfilingWidget(QDockWidget *parent);
};