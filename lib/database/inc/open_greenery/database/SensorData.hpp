#ifndef SENSOR_DATA_HPP
#define SENSOR_DATA_HPP

#include <cstdint>
#include <QDateTime>

namespace open_greenery
{
namespace database
{

struct SensorData
{
    QDateTime timestamp;
    std::uint16_t data;
};

}
}

#endif //SENSOR_DATA_HPP
