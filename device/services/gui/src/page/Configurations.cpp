#include "open_greenery/gui/page/Configurations.hpp"
#include <QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>

namespace open_greenery::gui::page
{

Configurations::Configurations()
    :m_main_widget(new QWidget())
{
    // Light button
    auto b_light = new QPushButton();
    b_light->setText("Light");
    QObject::connect(b_light, &QPushButton::pressed, this, &Configurations::toggleLight);
    auto page_layout = new QGridLayout();
    auto spacer1 = new QWidget();
    spacer1->setMinimumSize(1000, 200);
    auto spacer2 = new QWidget();
    spacer2->setMinimumSize(1000, 200);
    page_layout->addWidget(spacer1, 0, 0, 1, 6);
    page_layout->addWidget(b_light, 1, 2, 1, 2);
    page_layout->addWidget(spacer2, 2, 0, 1, 6);

    page_layout->addWidget(new QLabel("Pump pin:"), 3, 0);
    auto sb_pump_pin = new QSpinBox();
    sb_pump_pin->setMinimum(0);
    sb_pump_pin->setMaximum(31);
    page_layout->addWidget(sb_pump_pin, 4, 0);

    page_layout->addWidget(new QLabel("Soil Moisture sensor:"), 3, 1);
    auto cb_sm_sensors = new QComboBox();
    cb_sm_sensors->addItem("A0");
    cb_sm_sensors->addItem("A1");
    cb_sm_sensors->addItem("A2");
    cb_sm_sensors->addItem("A3");
    cb_sm_sensors->setCurrentIndex(3);
    page_layout->addWidget(cb_sm_sensors, 4, 1);

    page_layout->addWidget(new QLabel("Dry level:"), 3, 2);
    auto sb_dry = new QSpinBox();
    sb_dry->setMinimum(0);
    sb_dry->setMaximum(UINT16_MAX);
    sb_dry->setSingleStep(100);
    page_layout->addWidget(sb_dry, 4, 2);

    page_layout->addWidget(new QLabel("Wet level:"), 3, 3);
    auto sb_wet = new QSpinBox();
    sb_wet->setMinimum(0);
    sb_wet->setMaximum(UINT16_MAX);
    sb_wet->setSingleStep(100);
    page_layout->addWidget(sb_wet, 4, 3);

    page_layout->addWidget(new QLabel("Watering volume:"), 3, 4);
    auto sb_vol = new QSpinBox();
    sb_vol->setMinimum(0);
    page_layout->addWidget(sb_vol, 4, 4);

    page_layout->addWidget(new QLabel("Watering period:"), 3, 5);
    auto sb_period = new QSpinBox();
    sb_period->setMinimum(0);
    page_layout->addWidget(sb_period, 4, 5);

    m_main_widget->setLayout(page_layout);
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
