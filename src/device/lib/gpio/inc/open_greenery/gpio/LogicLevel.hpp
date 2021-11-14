#ifndef LOGIC_LEVEL_HPP
#define LOGIC_LEVEL_HPP

#include <wiringPi.h>

#undef LOW
#undef HIGH

namespace open_greenery::gpio
{

enum class LogicLevel : bool
{
    LOW = false,
    HIGH = true
};

LogicLevel operator!(LogicLevel value);

}

#endif //LOGIC_LEVEL_HPP
