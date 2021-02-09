#include "open_greenery/database/light/StatusWriter.hpp"
#include <SQLiteCpp/Transaction.h>
#include <utility>

namespace open_greenery::database::light
{

StatusWriter::StatusWriter(std::shared_ptr<SQLite::Database> _database)
    :LightDAO(std::move(_database))
{}

void StatusWriter::set(bool _is_enabled)
{
    SQLite::Transaction transaction(*m_database);

    // TODO: Remove hardcoded user_id
    const auto query = std::string("UPDATE light SET is_enabled = ") + (_is_enabled ? "1" : "0") + " WHERE user_id = 0;";
    m_database->exec(query);

    transaction.commit();
}

}
