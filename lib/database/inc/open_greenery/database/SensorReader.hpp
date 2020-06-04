#ifndef SENSOR_READER_HPP
#define SENSOR_READER_HPP

#include <vector>
#include <QDateTime>
#include "SensorRecord.hpp"
#include "Table.hpp"

namespace open_greenery
{
namespace database
{

class SensorReader
{
public:
    SensorReader(const Table _table);

    std::vector<SensorRecord> read(const QDateTime _from, const QDateTime _to);

    Table table() const;
private:
    const Table m_table;
};

}
}

#endif //SENSOR_READER_HPP
