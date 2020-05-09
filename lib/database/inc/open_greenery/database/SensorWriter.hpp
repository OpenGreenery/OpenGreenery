#ifndef SENSOR_WRITER_HPP
#define SENSOR_WRITER_HPP

#include <cstdint>
#include <SQLiteCpp/SQLiteCpp.h>
#include <QDateTime>
#include "Table.hpp"

namespace open_greenery
{
namespace database
{

class SensorWriter
{
public:
    SensorWriter(const Table _table);

    void write(const std::uint16_t _data);
    void write(const QDateTime timestamp, const std::uint16_t _data);
private:
    void write(const char * time, const std::uint16_t _data);
    const Table m_table;
};

}
}

#endif //SENSOR_WRITER_HPP
