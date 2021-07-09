#ifndef SENSOR_DATA_PROVIDER_HPP
#define SENSOR_DATA_PROVIDER_HPP

#include <vector>
#include <QDateTime>
#include "SensorRecord.hpp"

namespace open_greenery::dataflow
{

class ISensorDataProvider
{
public:
    virtual std::vector<SensorRecord> read(QDateTime _from, QDateTime _to) const = 0;

    ISensorDataProvider() = default;
    virtual ~ISensorDataProvider() = default;
    ISensorDataProvider(const ISensorDataProvider &) = delete;
    ISensorDataProvider & operator=(const ISensorDataProvider &) = delete;
};

}

#endif //SENSOR_DATA_PROVIDER_HPP
