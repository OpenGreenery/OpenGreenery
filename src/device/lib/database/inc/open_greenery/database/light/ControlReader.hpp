#ifndef OPEN_GREENERY_CONTROLREADER_HPP
#define OPEN_GREENERY_CONTROLREADER_HPP

#include "LightDao.hpp"
#include "open_greenery/dataflow/light/Participants.hpp"
#include <SQLiteCpp/Database.h>

namespace open_greenery::database::light
{

class ControlReader : public LightDAO, public open_greenery::dataflow::light::IManualControlOptionalProvider
{
public:
    explicit ControlReader(std::shared_ptr<SQLite::Database> _database);

    std::optional<open_greenery::dataflow::light::Control> get() override;
};

}

#endif //OPEN_GREENERY_CONTROLREADER_HPP
