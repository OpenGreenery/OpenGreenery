#ifndef SENSOR_READER_HPP
#define SENSOR_READER_HPP

#include <vector>
#include <QDateTime>
#include "DatabaseEntity.hpp"
#include "SensorRecord.hpp"

namespace open_greenery
{
namespace database
{

class SensorReader : public DatabaseEntity
{
public:
    SensorReader(const Table _table);

    std::vector<SensorRecord> read(const QDateTime _from, const QDateTime _to);
};

}
}

#endif //SENSOR_READER_HPP
