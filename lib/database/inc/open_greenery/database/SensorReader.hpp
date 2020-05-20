#ifndef SENSOR_READER_HPP
#define SENSOR_READER_HPP

#include <vector>
#include <QDateTime>
#include "SensorData.hpp"
#include "Table.hpp"

namespace open_greenery
{
namespace database
{

class SensorReader
{
public:
    SensorReader(const Table _table);

    std::vector<SensorData> read(const QDateTime from, const QDateTime to);

    Table table() const;
private:
    const Table m_table;
};

}
}

#endif //SENSOR_READER_HPP
