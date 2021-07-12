#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <functional>
#include <memory>
#include <QtWidgets/QMainWindow>
#include <QStackedLayout>
#include <QBoxLayout>
#include "open_greenery/gui/Topbar.hpp"
#include "open_greenery/gui/IAppQuit.hpp"
#include "open_greenery/gui/IPage.hpp"
#include "open_greenery/gui/IPageControl.hpp"

namespace open_greenery::gui
{

class MainWindow : public IAppQuit, public IPageControl
{
public:
    MainWindow(std::function<void()> _cb_quit_app);

public slots:
    void quit() override;
    void showConfigurations() override;
    void showMultipleCharts() override;

private:
    QMainWindow m_window_main;
    std::function<void()> m_cb_quit_app;
    Topbar m_topbar;
    QStackedLayout * m_page_layout;
    std::unique_ptr<IPage> m_page_soil;
    std::unique_ptr<IPage> m_page_config;
};

}

#endif //MAIN_WINDOW_HPP
