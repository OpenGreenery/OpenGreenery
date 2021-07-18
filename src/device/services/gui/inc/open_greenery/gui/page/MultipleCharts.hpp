#ifndef MULTIPLE_CHARTS_HPP
#define MULTIPLE_CHARTS_HPP

#include <QtWidgets/QWidget>
#include "open_greenery/gui/IPage.hpp"
#include "open_greenery/gui/page/DateValueChart.hpp"

namespace open_greenery::gui::page
{

class MultipleCharts final: public IPage
{
public:
    MultipleCharts(std::vector<DateValueChart> _charts);

    void update();
    void update(const QDateTime & _from, const QDateTime & _to);

    QWidget * widget() override;
private:
    QWidget * m_main_widget;
    std::vector<DateValueChart> m_charts;
};

}

#endif //MULTIPLE_CHARTS_HPP
