#include "open_greenery/database/SensorWriter.hpp"
#include <exception>
#include <functional>

namespace open_greenery
{
namespace database
{

SensorWriter::SensorWriter(std::shared_ptr<SQLite::Database> _db, const std::string _table_name)
    :m_db(_db),
    m_table_name(_table_name)
{
    m_db->exec("CREATE TABLE IF NOT EXISTS "+_table_name+" (time DATETIME PRIMARY KEY NOT NULL, value INTEGER NOT NULL)");
}

void SensorWriter::write(const std::uint16_t _data)
{
    SQLite::Transaction transaction(*m_db);
    int rv = m_db->exec("INSERT INTO "+m_table_name+
            " VALUES ((SELECT strftime('%Y-%m-%d %H:%M:%f', 'now')), "+
            std::to_string(_data)+")");

    if (rv != 1)
    {
        throw std::runtime_error("Database Error: Insert " + std::to_string(_data) +
                                 " to the " + m_table_name + " returned " + std::to_string(rv));
    }
    transaction.commit();
}

}
}
