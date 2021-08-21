#ifndef SENSOR_RECORD_HPP
#define SENSOR_RECORD_HPP

#include <cstdint>
#include <QDateTime>

namespace open_greenery::dataflow::irrigation
{

struct SensorRecord
{
    QDateTime timestamp;
    std::int16_t value;

    bool operator == (const SensorRecord & r) const
    {
        return (timestamp == r.timestamp) && (value == r.value);
    }

    bool operator != (const SensorRecord & r) const
    {
        return !(*this==r);
    }
};

}

#endif //SENSOR_RECORD_HPP
