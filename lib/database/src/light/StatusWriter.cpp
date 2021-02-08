#include "open_greenery/database/light/StatusWriter.hpp"
#include <QtDebug>
#include <QTime>

namespace open_greenery::database::light
{

void StatusWriter::set(bool _is_enabled)
{
    // TODO: Write flag to DB instead of log
    qDebug() << QTime::currentTime().toString() << "light status: " << (_is_enabled ? "ENABLED" : "DISABLED");
}

}
