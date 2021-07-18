#ifndef DATABASE_ENTITY_HPP
#define DATABASE_ENTITY_HPP

#include "Table.hpp"

namespace open_greenery::database
{

class DatabaseEntity
{
protected:
    DatabaseEntity(Table _table);

    Table table() const;
private:
    Table m_table;
};

}

#endif //DATABASE_ENTITY_HPP
