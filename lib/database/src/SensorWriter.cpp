#include <open_greenery/database/SensorWriter.hpp>
#include <exception>
#include <functional>

namespace open_greenery::database
{

SensorWriter::SensorWriter(Table _table)
    :DatabaseEntity(std::move(_table))
{
    table().database->exec("CREATE TABLE IF NOT EXISTS "+ table().name +
        " (time DATETIME PRIMARY KEY NOT NULL, value INTEGER NOT NULL)");
}

void SensorWriter::write(const std::int16_t _data)
{
    write("now", _data);
}

void SensorWriter::write(const open_greenery::dataflow::SensorRecord _record)
{
    write(_record.timestamp, _record.value);
}

void SensorWriter::write(const QDateTime _timestamp, const std::int16_t _data)
{
    const auto time_utc_str = _timestamp.toTimeSpec(Qt::UTC)
                              .toString("yyyy-MM-dd hh:mm:ss")
                              .toStdString();

    write(time_utc_str.c_str(), _data);
}

void SensorWriter::write(const char * _time, const std::int16_t _data)
{
    const auto data_str = std::to_string(_data);
    SQLite::Transaction transaction(*table().database);

    int rv = table().database->exec("INSERT INTO " + table().name + " VALUES "+
            "((SELECT strftime('%Y-%m-%d %H:%M:%f', '" + _time + "')), " + data_str + ")");

    if (rv != 1)
    {
        throw std::runtime_error("Database Error: Insert " + data_str +
                                 " to the " + table().name + " returned " + std::to_string(rv));
    }
    transaction.commit();
}

}
