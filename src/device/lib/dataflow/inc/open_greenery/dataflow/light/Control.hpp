#ifndef LIGHT_CONTROL_HPP
#define LIGHT_CONTROL_HPP

namespace open_greenery::dataflow::light
{

enum class Control : std::uint8_t
{
    ENABLE, DISABLE, TOGGLE
};

}

#endif //LIGHT_CONTROL_HPP
