#pragma once

#include <gmock/gmock.h>
#include <open_greenery/gpio/IOutputPin.hpp>

namespace open_greenery::mock::gpio
{

class OutputPinMock : public open_greenery::gpio::IOutputPin
{
public:
    MOCK_METHOD(void,
                write,
                (open_greenery::gpio::LogicLevel),
                (const, override));
};

}