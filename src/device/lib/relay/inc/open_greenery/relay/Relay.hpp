#ifndef RELAY_HPP
#define RELAY_HPP

#include <memory>
#include "IRelay.hpp"
#include "open_greenery/gpio/IOutputPin.hpp"

namespace open_greenery
{
namespace relay
{

class Relay : public IRelay
{
public:
    explicit Relay(std::shared_ptr<open_greenery::gpio::IOutputPin> _pin,
                   open_greenery::gpio::LogicLevel active_level = open_greenery::gpio::LogicLevel::HIGH);

    ~Relay() override;

    void enable() override;

    void disable() override;

    bool enabled() const override;

    void toggle() override;

private:
    bool m_enabled;
    std::shared_ptr<open_greenery::gpio::IOutputPin> m_gpio;
    const open_greenery::gpio::LogicLevel m_active_level;
};

}
}


#endif //RELAY_HPP
