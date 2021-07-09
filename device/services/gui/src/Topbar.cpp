#include "open_greenery/gui/Topbar.hpp"
#include <QPushButton>
#include <QLabel>

namespace open_greenery::gui
{

Topbar::Topbar(IAppQuit *_parent_quit, IPageControl *_parent_page_ctl)
    : m_layout(new QHBoxLayout),
      m_parent_quit(_parent_quit),
      m_parent_page_ctl(_parent_page_ctl)
{
    // OpenGreenery label
    auto l_og = new QLabel();
    QFont font;
    font.setBold(true);
    l_og->setFont(font);
    l_og->setText("OpenGreenery");
    m_layout->addWidget(l_og);
    // Quit button
    auto b_quit = new QPushButton();
    b_quit->setText("Quit");
    QObject::connect(b_quit, &QPushButton::pressed, this, &Topbar::quit);
    // Soil Moisture button
    auto b_moisture = new QPushButton();
    b_moisture->setText("Soil Moisture");
    QObject::connect(b_moisture, &QPushButton::pressed, this, &Topbar::showMultipleCharts);
    // Configurations button
    auto b_cfg = new QPushButton();
    b_cfg->setText("Configurations");
    QObject::connect(b_cfg, &QPushButton::pressed, this, &Topbar::showConfigurations);

    m_layout->addWidget(b_moisture);
    m_layout->addWidget(b_cfg);
    m_layout->addWidget(b_quit);
}

QLayout * Topbar::layout() const
{
    return m_layout;
}

void Topbar::quit()
{
    m_parent_quit->quit();
}

void Topbar::showConfigurations()
{
    m_parent_page_ctl->showConfigurations();
}

void Topbar::showMultipleCharts()
{
    m_parent_page_ctl->showMultipleCharts();
}

}
