#include <open_greenery/irrigation/moisture/state_machine/State.hpp>

namespace open_greenery::irrigation::moisture
{

State::State(const open_greenery::dataflow::irrigation::IrrigationConfigRecord & _config)
    :m_config(_config)
{}

}
