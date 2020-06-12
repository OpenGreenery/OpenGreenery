#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <open_greenery/database/IrrigationConfigRecord.hpp>
#include "SystemState.hpp"

namespace open_greenery::irrigation
{

class State
{
public:
    virtual SystemState handleSoilMoisture(std::int16_t _moisture) = 0;
    virtual SystemState state() = 0;

protected:
    State(open_greenery::database::IrrigationConfigRecord _config);
    virtual ~State() = default;

    const open_greenery::database::IrrigationConfigRecord & config() const;

private:
    open_greenery::database::IrrigationConfigRecord m_config;
};

}

#endif //STATE_HPP
