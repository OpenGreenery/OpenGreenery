#include <thread>
#include <utility>
#include <open_greenery/irrigation/moisture/state_machine/DryState.hpp>

namespace open_greenery::irrigation::moisture
{

DryState::DryState(const open_greenery::dataflow::irrigation::IrrigationConfigRecord & _config,
                   std::shared_ptr<open_greenery::pump::IPump> _pump)
    :State(_config),
    m_pump(std::move(_pump))
{}

StateName DryState::handleSoilMoisture(std::int16_t _moisture)
{
    if (_moisture < m_config.wet)// If well watered
    {
        // Change state to WET
        return StateName::WET;
    }

    m_pump->water(m_config.watering_volume);
    return StateName::DRY;
}

StateName DryState::state()
{
    return StateName::DRY;
}

}
