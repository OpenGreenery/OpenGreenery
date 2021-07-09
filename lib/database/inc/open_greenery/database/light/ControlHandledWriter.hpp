#ifndef OPEN_GREENERY_CONTROLHANDLEDWRITER_HPP
#define OPEN_GREENERY_CONTROLHANDLEDWRITER_HPP

#include "LightDao.hpp"
#include <open_greenery/dataflow/light/IControlHandledReceiver.hpp>
#include <SQLiteCpp/Database.h>

namespace open_greenery::database::light
{

class ControlHandledWriter : public LightDAO,  public open_greenery::dataflow::light::IControlHandledReceiver
{
public:
    explicit ControlHandledWriter(std::shared_ptr<SQLite::Database> _database);

    void handled() override;
};

}

#endif //OPEN_GREENERY_CONTROLHANDLEDWRITER_HPP
