#include "FPSWidget.hpp"
#include "Config.hpp"
#include "PerfStats.hpp"
#include <QBoxLayout>
#include <QLabel>

FPSWidget::FPSWidget(QDockWidget *parent)
	: QWidget(parent)
	, m_parent(parent)
	, m_fpsMinLines(new QtCharts::QLineSeries())
	, m_chart(new QtCharts::QChart())
	, m_chartView(new QtCharts::QChartView(m_chart))
	, m_xAxisSize(30)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->setMargin(0);

	int idealFps = Config::instance().getProperties().getIntProperty("graphics.max_fps", 120);

	QtCharts::QLineSeries *idealSeries = new QtCharts::QLineSeries();
	idealSeries->setColor(QColor(80, 80, 255));
	for (int i = 0; i < m_xAxisSize; i++)
	{
		idealSeries->append(i, idealFps);
	}

	m_chart->legend()->hide();
	m_chart->addSeries(idealSeries);
	m_chart->addSeries(m_fpsMinLines.get());
	m_chart->createDefaultAxes();
	m_chart->axisX()->hide();
	m_chart->axisY()->setMax(2 * idealFps);
	m_chart->setTitle("FPS");

	QPen pen;
	pen.setWidth(2);
	pen.setColor(QColor(255, 80, 80));
	m_fpsMinLines->setPen(pen);

	PerfStats::instance().addFPSListener([this](auto d) { _onUpdate(d); });

	m_chartView->setRenderHint(QPainter::Antialiasing);
	layout->addWidget(m_chartView);
	setContentsMargins(0, 0, 0, 0);

	setLayout(layout);
}

void FPSWidget::_onUpdate(std::deque<int> *fpsTicks)
{
	if (!m_parent->isVisible() || 
		(m_parent->visibleRegion().isEmpty() && !m_parent->isFloating()))
	{
		return;
	} 
	int fpsPerRow = 1 + fpsTicks->size() / m_xAxisSize;

	m_fpsMinLines->clear();
	for (int i = 0; i < m_xAxisSize; i++)
	{
		float startPct = (float)i / m_xAxisSize;
		float endPct = (float)(i+1) / m_xAxisSize;
		int startIdx = startPct * fpsTicks->size();
		int endIdx = endPct * fpsTicks->size();
		int minFps = (*fpsTicks)[startIdx];
		for (int j = startIdx + 1; j < endIdx; j++)
		{
			minFps = min(minFps, (int)(*fpsTicks)[j]);
		}
		m_fpsMinLines->append(i, minFps);
	}
}