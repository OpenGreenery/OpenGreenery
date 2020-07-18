#ifndef TABLE_HPP
#define TABLE_HPP

#include <SQLiteCpp/SQLiteCpp.h>

namespace open_greenery::database
{

struct Table
{
    Table(std::shared_ptr<SQLite::Database> _database, std::string _name);

    bool valid() const;

    const std::shared_ptr<SQLite::Database> database;
    const std::string name;
};

}

#endif //TABLE_HPP
