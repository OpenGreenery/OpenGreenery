#include <open_greenery/database/Table.hpp>

namespace open_greenery::database
{

Table::Table(std::shared_ptr<SQLite::Database> _database, std::string _name)
    :database(std::move(_database)),
    name(std::move(_name))
{
    if (!database)
    {
        throw std::logic_error("Database ptr is null");
    }
    if (name.empty())
    {
        throw std::logic_error("Table name is empty");
    }
}

bool Table::valid() const
{
    return database->tableExists(name);
}

}
