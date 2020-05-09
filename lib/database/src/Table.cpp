#include <open_greenery/database/Table.hpp>

namespace open_greenery
{
namespace database
{

Table::Table(const std::shared_ptr<SQLite::Database> _database, const std::string _name)
    :database(_database),
    name(_name)
{
    if (!_database)
    {
        throw std::logic_error("Database ptr is null");
    }
    if (_name.empty())
    {
        throw std::logic_error("Table name is empty");
    }
}

}
}
