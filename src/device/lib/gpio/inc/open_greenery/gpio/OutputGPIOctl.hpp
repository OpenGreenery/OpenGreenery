#ifndef OUTPUT_GPIO_CTL_HPP
#define OUTPUT_GPIO_CTL_HPP

#include "GPIOFactory.hpp"
#include "GPIOctl.hpp"
#include "LogicLevel.hpp"
#include "IOutputPin.hpp"
#include "PinId.hpp"

namespace open_greenery
{
namespace gpio
{

class OutputGPIOctl final : public IOutputPin, public GPIOctl
{
public:
    OutputGPIOctl(const PinId _pin);
    ~OutputGPIOctl() override;
    void write(const LogicLevel _value) const override;
};

}
}

#endif //OUTPUT_GPIO_CTL_HPP
