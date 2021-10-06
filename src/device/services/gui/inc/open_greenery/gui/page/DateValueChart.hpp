#ifndef DATE_VALUECHART_HPP
#define DATE_VALUECHART_HPP

#include <memory>
#include <QChartView>
#include <QDateTime>
#include "open_greenery/dataflow/irrigation/Participants.hpp"

namespace open_greenery::gui::page
{

class DateValueChart
{
public:
    DateValueChart(std::unique_ptr<open_greenery::dataflow::irrigation::ISensorDataProvider> _provider, const QString & _title);

    void update();
    void update(const QDateTime & _from, const QDateTime & _to);

    QtCharts::QChartView * chartView() const;
private:
    std::unique_ptr<open_greenery::dataflow::irrigation::ISensorDataProvider> m_sensor_provider;
    QDateTime m_current_from, m_current_to;
    QtCharts::QChartView * m_chart_view;
    const QString m_title;
};

}

#endif //DATE_VALUECHART_HPP
