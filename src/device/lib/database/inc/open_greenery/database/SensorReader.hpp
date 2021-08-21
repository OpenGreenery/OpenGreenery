#ifndef SENSOR_READER_HPP
#define SENSOR_READER_HPP

#include <vector>
#include <QDateTime>
#include "DatabaseEntity.hpp"
#include <open_greenery/dataflow/irrigation/Participants.hpp>

namespace open_greenery::database
{

class SensorReader : public DatabaseEntity,
                     public open_greenery::dataflow::irrigation::ISensorDataProvider,
                     public open_greenery::dataflow::irrigation::ISensorReadProvider
{
public:
    SensorReader(Table _table);

    std::vector<open_greenery::dataflow::irrigation::SensorRecord> read(QDateTime _from, QDateTime _to) const override;
    std::int16_t get() override;
};

}

#endif //SENSOR_READER_HPP
