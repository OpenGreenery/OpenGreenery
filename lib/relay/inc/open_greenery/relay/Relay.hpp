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
    Relay(std::shared_ptr<open_greenery::gpio::IOutputPin> _pin);

    ~Relay() override;

    void enable() override;

    void disable() override;

    bool enabled() const override;

    void toggle() override;

private:
    bool m_enabled;
    std::shared_ptr<open_greenery::gpio::IOutputPin> m_gpio;
};

}
}


#endif //RELAY_HPP
