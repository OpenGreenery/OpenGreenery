#include <thread>
#include <open_greenery/irrigation/DryState.hpp>

namespace open_greenery::irrigation
{

open_greenery::irrigation::DryState::DryState(open_greenery::dataflow::IrrigationConfigRecord _config,
                                              open_greenery::pump::Pump & _pump)
    :State(std::move(_config)),
    m_pump(_pump)
{}

SystemState DryState::handleSoilMoisture(std::int16_t _moisture)
{
    if (_moisture < config().wet)// If well watered
    {
        // Change state to WET
        return SystemState::WET;
    }

    m_pump.water(config().watering_volume);
    std::this_thread::sleep_for(config().watering_period);
    return SystemState::DRY;
}

SystemState DryState::state()
{
    return SystemState::DRY;
}

}
