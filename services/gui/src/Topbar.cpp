#include "open_greenery/gui/Topbar.hpp"
#include <QPushButton>
#include <QLabel>

namespace open_greenery::gui
{

Topbar::Topbar(IAppQuit * _parent_quit)
    :m_sidebar_layout(new QHBoxLayout),
    m_parent_quit(_parent_quit)
{
    // OpenGreenery label
    auto l_og = new QLabel();
    QFont font;
    font.setBold(true);
    l_og->setFont(font);
    l_og->setText("OpenGreenery");
    m_sidebar_layout->addWidget(l_og);
    // Quit button
    auto b_quit = new QPushButton();
    b_quit->setText("Quit");
    connect(b_quit, &QPushButton::pressed, this, &Topbar::quit);
    m_sidebar_layout->addWidget(b_quit);
}

QLayout * Topbar::layout() const
{
    return m_sidebar_layout;
}

void Topbar::quit()
{
    m_parent_quit->quit();
}

}
