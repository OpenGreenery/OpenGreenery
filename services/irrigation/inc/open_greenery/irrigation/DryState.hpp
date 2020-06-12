#ifndef DRY_STATE_HPP
#define DRY_STATE_HPP

#include <open_greenery/pump/Pump.hpp>
#include "State.hpp"

namespace open_greenery::irrigation
{

class DryState final : public State
{
public:
    DryState(open_greenery::database::IrrigationConfigRecord _config, open_greenery::pump::Pump & _pump);
    SystemState handleSoilMoisture(std::int16_t _moisture) override;
    SystemState state() override;

private:
    open_greenery::pump::Pump & m_pump;
};

}

#endif //DRY_STATE_HPP
