#ifndef OPEN_GREENERY_STATUSWRITER_HPP
#define OPEN_GREENERY_STATUSWRITER_HPP

#include "LightDao.hpp"
#include <open_greenery/dataflow/relay/Participants.hpp>
#include <SQLiteCpp/Database.h>


namespace open_greenery::database::light
{

class StatusWriter : public LightDAO, public open_greenery::dataflow::relay::IRelayStatusReceiver
{
public:
    explicit StatusWriter(std::shared_ptr<SQLite::Database> _database);

    void set(bool _is_enabled) override;
};

}

#endif //OPEN_GREENERY_STATUSWRITER_HPP
