#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <functional>
#include <QtWidgets/QMainWindow>
#include <QBoxLayout>
#include "open_greenery/gui/Topbar.hpp"
#include "open_greenery/gui/IAppQuit.hpp"
#include "open_greenery/gui/IPage.hpp"

namespace open_greenery::gui
{

class MainWindow : public IAppQuit
{
public:
    MainWindow(std::function<void()> _cb_quit_app);

public slots:
    void quit() override;

private:
    QMainWindow m_window_main;
    std::function<void()> m_cb_quit_app;
    Topbar m_topbar;
    IPage * m_page;
};

}

#endif //MAIN_WINDOW_HPP
