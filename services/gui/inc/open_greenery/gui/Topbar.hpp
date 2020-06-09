#ifndef TOPBAR_HPP
#define TOPBAR_HPP

#include <QVBoxLayout>
#include <open_greenery/gui/IAppQuit.hpp>
#include <open_greenery/gui/IPageControl.hpp>

namespace open_greenery::gui
{

class Topbar : public QObject //public IAppQuit//, public IPageControl
{
public:
    Topbar(IAppQuit * _parent_quit, IPageControl * _parent_page_ctl);

    QLayout * layout() const;

public slots:
    void quit();// override;
    void showConfigurations();// override;
    void showMultipleCharts();// override;

private:
    QLayout * m_layout;
    IAppQuit * m_parent_quit;
    IPageControl * m_parent_page_ctl;
};

}

#endif //TOPBAR_HPP
