#include <open_greenery/database/DatabaseEntity.hpp>

open_greenery::database::DatabaseEntity::DatabaseEntity(Table _table)
    :m_table(std::move(_table))
{}

open_greenery::database::Table open_greenery::database::DatabaseEntity::table() const
{
    return m_table;
}
