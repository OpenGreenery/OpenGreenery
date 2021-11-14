#include "open_greenery/relay/Relay.hpp"
#include <cassert>
#include <stdexcept>


namespace open_greenery
{
namespace relay
{

namespace ogio = open_greenery::gpio;

Relay::Relay(std::shared_ptr<open_greenery::gpio::IOutputPin> _pin,
             open_greenery::gpio::LogicLevel _active_level)
     :m_enabled(false),
     m_gpio(std::move(_pin)),
     m_active_level(_active_level)
{
    assert(m_gpio && "Output pin pointer in null");
    m_gpio->write(!m_active_level);
}

Relay::~Relay()
{
    // FIXME: Virtual method invocation in destructor
    disable();
}

void Relay::enable()
{
    m_gpio->write(m_active_level);
    m_enabled = true;
}

void Relay::disable()
{
    m_gpio->write(!m_active_level);
    m_enabled = false;
}

bool Relay::enabled() const
{
    return m_enabled;
}

void Relay::toggle()
{
    if (enabled())
    {
        disable();
    }
    else
    {
        enable();
    }
}

}
}
