#include "open_greenery/pump/Pump.hpp"
#include <cassert>
#include <thread>
#include <open_greenery/gpio/GPIOFactory.hpp>

namespace open_greenery
{
namespace pump
{

namespace ogio = open_greenery::gpio;

Pump::Pump(std::shared_ptr<open_greenery::relay::IRelay> _relay)
        : m_relay(std::move(_relay))
{
    assert(m_relay && "Relay is nullptr");
}

void Pump::water(const std::chrono::milliseconds _dur)
{
    m_relay->enable();
    std::this_thread::sleep_for(_dur);
    m_relay->disable();
}

void Pump::water(const std::uint16_t _vol_ml)
{
    constexpr float ML_MIN {100.};
    std::chrono::duration<float, std::ratio<60, 1>> watering_dur (float(_vol_ml)/ML_MIN);
    water(std::chrono::duration_cast<std::chrono::milliseconds>(watering_dur));
}


}
}
