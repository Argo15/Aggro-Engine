#include "MemoryWidget.hpp"
#include <QChart>
#include <QtCharts\QChartView>
#include <QBoxLayout>
#include <QLabel>
#include <boost/range/adaptor/map.hpp> 
#include "PerfStats.hpp"

MemoryWidget::MemoryWidget(QDockWidget *parent)
	: QWidget(parent)
	, m_chart(new QtCharts::QChart())
	, m_barSet(new QtCharts::QBarSet("Memory"))
	, m_barSeries(new QtCharts::QHorizontalStackedBarSeries())
	, m_axis(new QtCharts::QBarCategoryAxis())
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	m_barSeries->append(m_barSet.get());

	if (m_chart->series().size() == 0)
	{
		m_chart->addSeries(m_barSeries.get());
	}

	m_chart->createDefaultAxes();
	m_chart->axisX()->setRange(0, 2048);
	m_chart->setAxisY(m_axis.get(), m_barSeries.get());

	QtCharts::QChartView *chartView = new QtCharts::QChartView(m_chart.get());
	chartView->setRenderHint(QPainter::Antialiasing);

	m_chart->legend()->setVisible(false);
	m_chart->setTitle("Bytes allocated MB");

	layout->addWidget(chartView);
	setLayout(layout);

	PerfStats::instance().addMemoryListener([this](auto map) { _onUpdate(*map); });
}

void MemoryWidget::_onUpdate(boost::unordered_map<string, long> &nameToBytesAllocated)
{
	long maxBytes = 1;
	QStringList newCategories;
	for (string cat : (nameToBytesAllocated | boost::adaptors::map_keys))
	{
		long curMB = nameToBytesAllocated[cat] / 1024 / 1024;
		QString category = QString::fromStdString(cat);
		if (m_axis->categories().contains(category))
		{
			// update existing categories
			int idx = m_axis->categories().indexOf(category);
			m_barSet->replace(idx, curMB);
		}
		else
		{
			// add new categories
			m_axis->append(QString::fromStdString(cat));
			*m_barSet << curMB;
		}
		maxBytes = max(maxBytes, curMB);
	}
	m_chart->axisX()->setRange(0, maxBytes);
}