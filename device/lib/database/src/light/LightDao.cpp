#include "open_greenery/database/light/LightDao.hpp"
#include <cassert>
#include <SQLiteCpp/Transaction.h>


namespace open_greenery::database::light
{

LightDAO::LightDAO(std::shared_ptr<SQLite::Database> _database)
    :m_database(std::move(_database))
{
    assert(m_database);

    SQLite::Transaction transaction(*m_database);
    auto query = std::string("CREATE TABLE IF NOT EXISTS light (") +
            "id INTEGER PRIMARY KEY AUTOINCREMENT," +
            "user_id INTEGER NOT NULL UNIQUE," +
            "mode TEXT NOT NULL CHECK ( mode IN ('manual', 'auto') )," +
            "day_start TIME," +
            "day_end TIME," +
            "is_enabled BOOLEAN NOT NULL," +
            "control TEXT NOT NULL CHECK( control IN ('handled', 'enable', 'disable', 'toggle')));";
    m_database->exec(query);

    transaction.commit();

    // Is the table empty
    query = "select count(*) as count from light;";
    SQLite::Statement query2(*m_database, query);
    query2.executeStep();
    const int count = query2.getColumn("count");
    if (count != 0)
    {
        // Default configuration not needed
        return;
    }

    // Default configuration
    SQLite::Transaction transaction2(*m_database);

    // TODO: Remove hardcoded user_id and time configurations
    // TODO: Implement mode: DAO and handling
    // TODO: time configurations DAO
    query = std::string("INSERT INTO light (mode, user_id, day_start, day_end, is_enabled, control) VALUES (") +
            "'auto'," +
            "0," +
            "(SELECT strftime('08', '00'))," +
            "(SELECT strftime('22', '00'))," +
            "false," +
            "'handled');";
    m_database->exec(query);

    transaction2.commit();
}

}
