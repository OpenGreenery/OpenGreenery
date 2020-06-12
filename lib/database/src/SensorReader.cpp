#include "open_greenery/database/SensorReader.hpp"
#include <SQLiteCpp/VariadicBind.h>

namespace open_greenery
{
namespace database
{

SensorReader::SensorReader(const Table _table)
    :DatabaseEntity(std::move(_table))
{}

std::vector<SensorRecord> SensorReader::read(const QDateTime _from, const QDateTime _to)
{
    auto utc_str = [](const QDateTime & _dt) -> std::string
    {
        return _dt.toTimeSpec(Qt::UTC)
                  .toString("yyyy-MM-dd hh:mm:ss")
                  .toStdString();
    };

    const std::string strftime_format ("'%Y-%m-%d %H:%M:%f'");
    SQLite::Statement query(*table().database, "SELECT * FROM "+table().name+" WHERE "+
        "(time > strftime("+strftime_format+", '"+utc_str(_from)+"') AND "+
        "time < strftime("+strftime_format+", '"+utc_str(_to)+"'));");

    std::vector<SensorRecord> rv;
    while (query.executeStep())
    {
        const char * timestamp = query.getColumn("time");
        const char * value = query.getColumn("value");

        auto utc_dt = QDateTime::fromString(QString(timestamp), "yyyy-MM-dd hh:mm:ss.zzz");
        utc_dt.setTimeSpec(Qt::UTC);// SQLite stores datetime in UTC

        auto local_dt = utc_dt.toLocalTime();
        rv.push_back({local_dt, std::int16_t(std::atoi(value))});
    }
    query.reset();

    rv.shrink_to_fit();
    return rv;
}

SensorRecord SensorReader::readLast()
{
    std::string str_query = "SELECT * FROM "
            + table().name
            + " WHERE time == (SELECT MAX(time) FROM "
            + table().name
            +")";
    SQLite::Statement query(*table().database, str_query);
    query.executeStep();
    const char * timestamp = query.getColumn("time");
    const char * value = query.getColumn("value");

    auto utc_dt = QDateTime::fromString(QString(timestamp), "yyyy-MM-dd hh:mm:ss.zzz");
    utc_dt.setTimeSpec(Qt::UTC);// SQLite stores datetime in UTC

    return {utc_dt.toLocalTime(), std::int16_t(std::atoi(value))};
}

}
}
