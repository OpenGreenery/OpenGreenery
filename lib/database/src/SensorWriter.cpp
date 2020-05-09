#include "open_greenery/database/SensorWriter.hpp"
#include <exception>
#include <functional>

namespace open_greenery
{
namespace database
{

SensorWriter::SensorWriter(const Table _table)
    :m_table(_table)
{
    m_table.database->exec("CREATE TABLE IF NOT EXISTS "+ _table.name +
        " (time DATETIME PRIMARY KEY NOT NULL, value INTEGER NOT NULL)");
}

void SensorWriter::write(const std::uint16_t _data)
{
    write("now", _data);
}

void SensorWriter::write(const QDateTime timestamp, const std::uint16_t _data)
{
    const auto time = timestamp.toTimeSpec(Qt::UTC)
                               .toString("yyyy-MM-dd hh:mm:ss")
                               .toStdString()
                               .c_str();

    write(time, _data);
}

void SensorWriter::write(const char * time, const std::uint16_t _data)
{
    const auto data_str = std::to_string(_data);
    SQLite::Transaction transaction(*m_table.database);

    int rv = m_table.database->exec("INSERT INTO " + m_table.name + " VALUES "+
            "((SELECT strftime('%Y-%m-%d %H:%M:%f', '" + time + "')), " + data_str + ")");

    if (rv != 1)
    {
        throw std::runtime_error("Database Error: Insert " + data_str +
                                 " to the " + m_table.name + " returned " + std::to_string(rv));
    }
    transaction.commit();
}

}
}
