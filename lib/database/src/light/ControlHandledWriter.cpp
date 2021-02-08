#include "open_greenery/database/light/ControlHandledWriter.hpp"
#include <QDebug>
#include <QTime>

namespace open_greenery::database::light
{

void ControlHandledWriter::handled()
{
    // TODO: Write flag to DB instead of log
    qDebug() << QTime::currentTime().toString() << "light manual ctl set: handled";
}

}
