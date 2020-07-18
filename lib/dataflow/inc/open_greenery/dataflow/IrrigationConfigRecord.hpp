#ifndef IRRIGATION_CONFIG_RECORD_HPP
#define IRRIGATION_CONFIG_RECORD_HPP

#include <chrono>
#include <string>
#include <open_greenery/gpio/PinId.hpp>

namespace open_greenery::dataflow
{

struct IrrigationConfigRecord
{
    open_greenery::gpio::PinId pin;
    std::int16_t dry;
    std::int16_t wet;
    std::uint16_t watering_volume;
    std::chrono::seconds watering_period;
    std::string soil_moisture_sensor;
};

}

#endif //IRRIGATION_CONFIG_RECORD_HPP
