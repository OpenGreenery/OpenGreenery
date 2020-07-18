#ifndef SENSOR_DATA_RECEIVER_HPP
#define SENSOR_DATA_RECEIVER_HPP

#include <vector>
#include <QDateTime>
#include "SensorRecord.hpp"

namespace open_greenery::dataflow
{

class ISensorDataReceiver
{
public:
    virtual void write(std::int16_t _data) = 0;
    virtual void write(SensorRecord _record) = 0;
    virtual void write(QDateTime _timestamp, std::int16_t _data) = 0;

    ISensorDataReceiver() = default;
    virtual ~ISensorDataReceiver() = default;
    ISensorDataReceiver(const ISensorDataReceiver &) = delete;
    ISensorDataReceiver & operator=(const ISensorDataReceiver &) = delete;
};

}

#endif //SENSOR_DATA_RECEIVER_HPP
