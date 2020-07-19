#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <open_greenery/dataflow/IrrigationConfigRecord.hpp>
#include "SystemState.hpp"

namespace open_greenery::irrigation
{

class State
{
public:
    virtual SystemState handleSoilMoisture(std::int16_t _moisture) = 0;
    virtual SystemState state() = 0;

protected:
    State(open_greenery::dataflow::IrrigationConfigRecord _config);
    virtual ~State() = default;

    const open_greenery::dataflow::IrrigationConfigRecord & config() const;

private:
    open_greenery::dataflow::IrrigationConfigRecord m_config;
};

}

#endif //STATE_HPP
