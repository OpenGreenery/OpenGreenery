#ifndef TABLE_HPP
#define TABLE_HPP

#include <SQLiteCpp/SQLiteCpp.h>

namespace open_greenery
{
namespace database
{

struct Table
{
    Table(const std::shared_ptr<SQLite::Database> _database, const std::string _name);

    const std::shared_ptr<SQLite::Database> database;
    const std::string name;
};

}
}

#endif //TABLE_HPP
