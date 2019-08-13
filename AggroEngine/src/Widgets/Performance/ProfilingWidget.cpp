#include "ProfilingWidget.hpp"
#include <QChart>
#include <QtCharts\QChartView>
#include <QBoxLayout>
#include <QLabel>
#include <boost/range/adaptor/map.hpp> 
#include "PerfStats.hpp"

ProfilingWidget::ProfilingWidget(QDockWidget *parent)
	: QWidget(parent)
	, m_parent(parent)
	, m_chart(new QtCharts::QChart())
	, m_maxBarSet(new QtCharts::QBarSet("Max"))
	, m_avgBarSet(new QtCharts::QBarSet("Average"))
	, m_barSeries(new QtCharts::QHorizontalBarSeries())
	, m_axis(new QtCharts::QBarCategoryAxis())
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	m_barSeries->append(m_maxBarSet.get());
	m_barSeries->append(m_avgBarSet.get());

	if (m_chart->series().size() == 0)
	{
		m_chart->addSeries(m_barSeries.get());
	}

	m_chart->createDefaultAxes();
	m_chart->axisX()->setRange(0, 20);
	m_chart->setAxisY(m_axis.get(), m_barSeries.get());

	QtCharts::QChartView *chartView = new QtCharts::QChartView(m_chart.get());
	chartView->setRenderHint(QPainter::Antialiasing);

	m_chart->legend()->setVisible(false);
	m_chart->setTitle("Time tracking milliseconds");

	layout->addWidget(chartView);
	setLayout(layout);

	PerfStats::instance().addProfilingListener([this](auto map) { _onUpdate(*map); });
}

void ProfilingWidget::_onUpdate(boost::unordered_map<string, shared_ptr<ProfilingData>> &nameToProfilingData)
{
	if (!m_parent->isVisible() ||
		(m_parent->visibleRegion().isEmpty() && !m_parent->isFloating()))
	{
		return;
	}

	double maxTime = 1.0;
	QStringList newCategories;
	for (string cat : (nameToProfilingData | boost::adaptors::map_keys))
	{
		shared_ptr<ProfilingData> profilingData  = nameToProfilingData[cat];
		QString category = QString::fromStdString(cat);
		if (m_axis->categories().contains(category))
		{
			// update existing categories
			int idx = m_axis->categories().indexOf(category);
			m_avgBarSet->replace(idx, profilingData->getAverageTime() / 1000.0);
			//m_maxBarSet->replace(idx, profilingData->getMaxTime() / 1000.0);
		}
		else
		{
			// add new categories
			m_axis->append(QString::fromStdString(cat));
			*m_avgBarSet << profilingData->getAverageTime() / 1000.0;
			//*m_maxBarSet << profilingData->getMaxTime() / 1000.0;
		}
		maxTime = max(maxTime, profilingData->getAverageTime() / 1000.0);
	}
	int minRange = 10;
	while (minRange < maxTime)
	{
		minRange *= 2;
	}
	m_chart->axisX()->setRange(0, minRange);
}