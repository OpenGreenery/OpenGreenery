#include "open_greenery/database/light/ControlHandledWriter.hpp"
#include <SQLiteCpp/Transaction.h>
#include <utility>


namespace open_greenery::database::light
{

ControlHandledWriter::ControlHandledWriter(std::shared_ptr<SQLite::Database> _database)
        :LightDAO(std::move(_database))
{}

void ControlHandledWriter::handled()
{
    SQLite::Transaction transaction(*m_database);

    // TODO: Remove hardcoded user_id
    m_database->exec("UPDATE light SET control = 'handled' WHERE user_id = 0;");

    transaction.commit();
}

}
