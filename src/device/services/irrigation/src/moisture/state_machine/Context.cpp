#include <open_greenery/irrigation/moisture/state_machine/Context.hpp>
#include <open_greenery/irrigation/moisture/state_machine/DryState.hpp>
#include <open_greenery/irrigation/moisture/state_machine/WetState.hpp>

namespace open_greenery::irrigation::moisture
{

Context::Context(const open_greenery::dataflow::irrigation::IrrigationConfigRecord & _config,
                 std::shared_ptr<open_greenery::pump::IPump> _pump)
    :m_states({
        {StateName::DRY, std::make_shared<DryState>(_config, _pump)},
        {StateName::WET, std::make_shared<WetState>(_config)}
    }),
    m_current_state(m_states[StateName::WET])
{}

StateName Context::state()
{
    return m_current_state->state();
}

StateName Context::handleSoilMoisture(std::int16_t _moisture)
{
    const auto next_state = m_current_state->handleSoilMoisture(_moisture);
    m_current_state = m_states[next_state];
    return next_state;
}

}
