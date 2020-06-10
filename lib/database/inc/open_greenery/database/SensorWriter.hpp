#ifndef SENSOR_WRITER_HPP
#define SENSOR_WRITER_HPP

#include <cstdint>
#include <SQLiteCpp/SQLiteCpp.h>
#include <QDateTime>
#include "DatabaseEntity.hpp"
#include "SensorRecord.hpp"

namespace open_greenery
{
namespace database
{

class SensorWriter : public DatabaseEntity
{
public:
    SensorWriter(Table _table);

    void write(const std::int16_t _data);
    void write(const SensorRecord _record);
    void write(const QDateTime _timestamp, const std::int16_t _data);

private:
    void write(const char * _time, const std::int16_t _data);
};

}
}

#endif //SENSOR_WRITER_HPP
