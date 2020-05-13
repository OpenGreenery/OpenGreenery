#ifndef SENSOR_WRITER_HPP
#define SENSOR_WRITER_HPP

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include "SQLiteCpp/SQLiteCpp.h"

namespace open_greenery
{
namespace database
{

class SensorWriter
{
public:
    SensorWriter(std::shared_ptr<SQLite::Database> _db, const std::string _table_name);

    void write(const std::int16_t _data);
private:
    const std::shared_ptr<SQLite::Database> m_db;
    const std::string m_table_name;
};

}
}

#endif //SENSOR_WRITER_HPP
