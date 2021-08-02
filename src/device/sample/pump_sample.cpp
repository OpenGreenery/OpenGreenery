#include <open_greenery/pump/Pump.hpp>
#include <open_greenery/relay/Relay.hpp>
#include <open_greenery/gpio/GPIOFactory.hpp>
#include <iostream>

namespace og = open_greenery;

int main()
{
    std::cout << "Create pump instance." << std::endl;
    auto pin = og::gpio::GPIOFactory::getInstance().getOutputGPIOctl(
        {7u, og::gpio::Pinout::WIRING_PI}
    );
    auto relay = std::make_shared<og::relay::Relay>(pin);
    og::pump::Pump pump (relay);
    std::cout << "Enter volume of watering in ml:" << std::endl;
    unsigned int volume {};
    std::cin >> volume;
    pump.water(volume);
    std::cout << "Disable pump." << std::endl;
    return 0;
}
