#include "open_greenery/gui/page/MultipleCharts.hpp"
#include <cmath>
#include <QGridLayout>

namespace open_greenery::gui::page
{

MultipleCharts::MultipleCharts(std::vector<DateValueChart> _charts)
    :m_main_widget(new QWidget),
    m_charts(std::move(_charts))
{
    auto charts_layout = new QGridLayout();

    const QDateTime date_from = QDateTime::currentDateTime().addMonths(-1);// Month ago
    const QDateTime date_to = QDateTime::currentDateTime();
    auto prepared_chart = m_charts.begin();
    const std::uint8_t square_size = ceil(sqrt(_charts.size()));
    for (std::uint8_t x {}; x < square_size; x++)
    {
        for (std::uint8_t y {}; y < square_size; y++)
        {
            if (prepared_chart == m_charts.end())
            {
                break;
            }
            prepared_chart->update(date_from, date_to);
            charts_layout->addWidget(prepared_chart->chartView(), y, x);
            prepared_chart++;
        }
    }
    m_main_widget->setLayout(charts_layout);
}

void MultipleCharts::update()
{
    for (auto & chart : m_charts)
    {
        chart.update();
    }
}

void MultipleCharts::update(const QDateTime & _from, const QDateTime & _to)
{
    for (auto & chart : m_charts)
    {
        chart.update(_from, _to);
    }
}

QWidget * MultipleCharts::widget()
{
    return m_main_widget;
}

}
