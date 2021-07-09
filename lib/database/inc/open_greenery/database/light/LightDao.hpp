#ifndef LIGHT_DAO_HPP
#define LIGHT_DAO_HPP

#include <memory>
#include <SQLiteCpp/Database.h>

namespace open_greenery::database::light
{

/**
 * Base class for data access objects of light service
 */
class LightDAO
{
protected:
    /**
     * Creates table if doesn't exist
     * @param _database pointer to database instance
     */
    explicit LightDAO(std::shared_ptr<SQLite::Database> _database);

    std::shared_ptr<SQLite::Database> m_database;
};

}

#endif //LIGHT_DAO_HPP
