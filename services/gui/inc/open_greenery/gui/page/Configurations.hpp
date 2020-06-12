#ifndef CONFIGURATIONS_HPP
#define CONFIGURATIONS_HPP

#include <QPushButton>
#include "open_greenery/gui/IPage.hpp"

namespace open_greenery::gui::page
{

class Configurations : public IPage, public QObject
{
public:
    Configurations();

    QWidget * widget() override;

private slots:
    void toggleLight();

private:
    QWidget * m_main_widget;
    bool m_light_state {};
};

}

#endif //CONFIGURATIONS_HPP
