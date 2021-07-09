#include <open_greenery/database/DatabaseEntity.hpp>

namespace open_greenery::database
{

DatabaseEntity::DatabaseEntity(Table _table)
        : m_table(std::move(_table)) {}

Table DatabaseEntity::table() const
{
    return m_table;
}

}
