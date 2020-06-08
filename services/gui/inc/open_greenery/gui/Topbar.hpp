#ifndef TOPBAR_HPP
#define TOPBAR_HPP

#include <QVBoxLayout>
#include <open_greenery/gui/IAppQuit.hpp>

namespace open_greenery::gui
{

class Topbar : public IAppQuit
{
public:
    Topbar(IAppQuit * _parent_quit);

    QLayout * layout() const;

public slots:
    void quit() override;

private:
    QLayout * m_sidebar_layout;
    IAppQuit * m_parent_quit;
};

}

#endif //TOPBAR_HPP
