#include "open_greenery/database/SensorReader.hpp"
#include <SQLiteCpp/VariadicBind.h>

namespace open_greenery
{
namespace database
{

SensorReader::SensorReader(const Table _table)
    :m_table(_table)
{}

std::vector<SensorData> SensorReader::read(const QDateTime from, const QDateTime to)
{
    auto utc_str = [](const QDateTime & _dt) -> std::string
    {
        return _dt.toTimeSpec(Qt::UTC)
                  .toString("yyyy-MM-dd hh:mm:ss")
                  .toStdString();
    };

    const std::string strftime_format ("'%Y-%m-%d %H:%M:%f'");
    SQLite::Statement query(*m_table.database, "SELECT * FROM "+m_table.name+" WHERE "+
        "(time > strftime("+strftime_format+", '"+utc_str(from)+"') AND "+
        "time < strftime("+strftime_format+", '"+utc_str(to)+"'));");

    std::vector<SensorData> rv;
    while (query.executeStep())
    {
        const char * timestamp = query.getColumn("time");
        const char * value = query.getColumn("value");

        auto utc_dt = QDateTime::fromString(QString(timestamp), "yyyy-MM-dd hh:mm:ss.zzz");
        utc_dt.setTimeSpec(Qt::UTC);// SQLite stores datetime in UTC

        auto local_dt = utc_dt.toLocalTime();
        rv.push_back({local_dt, std::uint16_t(std::atoi(value))});
    }
    query.reset();

    rv.shrink_to_fit();
    return rv;
}

Table SensorReader::table() const
{
    return m_table;
}

}
}
