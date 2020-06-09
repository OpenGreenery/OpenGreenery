#include "open_greenery/gui/page/Configurations.hpp"
#include <QVBoxLayout>

namespace open_greenery::gui::page
{


Configurations::Configurations()
    :m_main_widget(new QWidget())//,
    //m_light_relay({2, open_greenery::gpio::Pinout::WIRING_PI})
{
    auto b_light = new QPushButton();
    b_light->setText("Light");
    QObject::connect(b_light, &QPushButton::pressed, this, &Configurations::toggleLight);
    auto layout = new QVBoxLayout();
    layout->addWidget(b_light);
    m_main_widget->setLayout(layout);
}

QWidget * Configurations::widget()
{
    return m_main_widget;
}

void Configurations::toggleLight()
{
    if (m_light_state)
    {
        system("gpio write 2 0");
        m_light_state = false;
    }
    else
    {
        system("gpio write 2 1");
        m_light_state = true;
    }
//    m_light_relay.toggle();
}
}
