#include "open_greenery/gui/page/Configurations.hpp"
#include <QVBoxLayout>

namespace open_greenery::gui::page
{

Configurations::Configurations()
    :m_main_widget(new QWidget())
{
    auto b_light = new QPushButton();
    b_light->setText("Light");
    QObject::connect(b_light, &QPushButton::pressed, this, &Configurations::toggleLight);
    auto layout = new QVBoxLayout();
    layout->addWidget(b_light);
    m_main_widget->setLayout(layout);
    system("gpio mode 2 out");
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

}
}
