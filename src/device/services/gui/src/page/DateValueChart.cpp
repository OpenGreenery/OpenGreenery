#include "open_greenery/gui/page/DateValueChart.hpp"
#include <QChart>
#include <QtCharts/QSplineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>

namespace open_greenery::gui::page
{

DateValueChart::DateValueChart(
        std::unique_ptr<open_greenery::dataflow::irrigation::ISensorDataProvider> _provider,
        const QString & _title)
    : m_sensor_provider(std::move(_provider)),
      m_chart_view(new QtCharts::QChartView),
      m_title(_title)
{
    m_chart_view->setRenderHint(QPainter::Antialiasing);
}

void DateValueChart::update(const QDateTime & _from, const QDateTime & _to)
{
    m_current_from = _from;
    m_current_to = _to;

    auto series = new QtCharts::QSplineSeries();

    for (const auto & moisture_data : m_sensor_provider->read(_from, _to))
    {
        series->append(moisture_data.timestamp.toMSecsSinceEpoch(), moisture_data.value);
    }

    auto chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle(m_title);

    auto * axisX = new QtCharts::QDateTimeAxis();
    axisX->setFormat("dd.MM");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto * axisY = new QtCharts::QValueAxis();
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_chart_view->setChart(chart);
}

void DateValueChart::update()
{
    update(m_current_from, m_current_to);
}

QtCharts::QChartView * DateValueChart::chartView() const
{
    return m_chart_view;
}

}
