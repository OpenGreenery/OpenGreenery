#ifndef SENSOR_RECORD_HPP
#define SENSOR_RECORD_HPP

#include <cstdint>
#include <QDateTime>

namespace open_greenery::dataflow
{

struct SensorRecord
{
    QDateTime timestamp;
    std::int16_t value;
};

}

#endif //SENSOR_RECORD_HPP
