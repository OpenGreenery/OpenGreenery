#include "open_greenery/database/light/ControlReader.hpp"
#include <QDebug>
#include <QTime>

namespace open_greenery::database::light
{


std::optional<open_greenery::dataflow::light::Control> ControlReader::get()
{
    // TODO: Read control from DB instead of log
    qDebug() << QTime::currentTime().toString() << "light manual ctl get: handled (forced)";
    return std::optional<open_greenery::dataflow::light::Control>();
}

}
