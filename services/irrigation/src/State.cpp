#include <open_greenery/irrigation/State.hpp>
#include <utility>

namespace open_greenery::irrigation
{

State::State(open_greenery::dataflow::IrrigationConfigRecord _config)
    :m_config(std::move(_config))
{}

const open_greenery::dataflow::IrrigationConfigRecord & State::config() const
{
    return m_config;
}

}
