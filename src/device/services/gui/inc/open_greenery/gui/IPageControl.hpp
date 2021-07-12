#ifndef I_PAGE_CONTROL_HPP
#define I_PAGE_CONTROL_HPP

namespace open_greenery::gui
{

class IPageControl : public QObject
{
public slots:
    virtual void showConfigurations() = 0;
    virtual void showMultipleCharts() = 0;
};

}

#endif //I_PAGE_CONTROL_HPP
