#include "Relay.hpp"

namespace open_greenery::tests::services::light::mock
{

void Relay::enable()
{
    m_enabled = true;
}

void Relay::disable()
{
    m_enabled = false;
}

void Relay::toggle()
{
    m_enabled = !m_enabled;
}

bool Relay::enabled() const
{
    return m_enabled;
}

}
