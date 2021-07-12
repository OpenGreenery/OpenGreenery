#ifndef PIN_ID_HPP
#define PIN_ID_HPP

#include <cstdint>
#include "Pinout.hpp"

namespace open_greenery
{
namespace gpio
{

using PinNumber = std::uint8_t;

struct PinId
{
    PinNumber pin;
    Pinout pinout;

    PinId cast_to(Pinout _po) const;
};

}
}

#endif //PIN_ID_HPP
