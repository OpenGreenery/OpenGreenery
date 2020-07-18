#ifndef SENSOR_READER_HPP
#define SENSOR_READER_HPP

#include <vector>
#include <QDateTime>
#include "DatabaseEntity.hpp"
#include <open_greenery/dataflow/ISensorDataProvider.hpp>

namespace open_greenery::database
{

class SensorReader : public DatabaseEntity,
                     public open_greenery::dataflow::ISensorDataProvider
{
public:
    SensorReader(Table _table);

    std::vector<open_greenery::dataflow::SensorRecord> read(QDateTime _from, QDateTime _to) override;
    open_greenery::dataflow::SensorRecord readLast() override;
};

}

#endif //SENSOR_READER_HPP
