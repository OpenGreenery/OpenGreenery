#include "open_greenery/gpio/LogicLevel.hpp"

namespace open_greenery::gpio
{

LogicLevel operator!(LogicLevel value)
{
    return static_cast<LogicLevel>(!static_cast<bool>(value));
}

}
