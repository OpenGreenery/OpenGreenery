#include <open_greenery/irrigation/WetState.hpp>
#include <chrono>
#include <thread>

namespace open_greenery::irrigation
{

WetState::WetState(open_greenery::dataflow::IrrigationConfigRecord _config)
    :State(std::move(_config))
{}

SystemState WetState::state()
{
    return SystemState::WET;
}

SystemState WetState::handleSoilMoisture(std::int16_t _moisture)
{
    if (_moisture >= config().dry)// If soil is to dry
    {
        // Change machine state to DRY
        return SystemState::DRY;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return SystemState::WET;
}

}
