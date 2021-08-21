#ifndef SENSOR_WRITER_HPP
#define SENSOR_WRITER_HPP

#include <cstdint>
#include <SQLiteCpp/SQLiteCpp.h>
#include <QDateTime>
#include "DatabaseEntity.hpp"
#include <open_greenery/dataflow/irrigation/Participants.hpp>

namespace open_greenery::database
{

class SensorWriter : public DatabaseEntity,
                     public open_greenery::dataflow::irrigation::ISensorDataReceiver
{
public:
    SensorWriter(Table _table);

    void write(std::int16_t _data) override;
    void write(open_greenery::dataflow::irrigation::SensorRecord _record) override;
    void write(QDateTime _timestamp, std::int16_t _data) override;

private:
    void write(const char * _time, std::int16_t _data);
};

}

#endif //SENSOR_WRITER_HPP
