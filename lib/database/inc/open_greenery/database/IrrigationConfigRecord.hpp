#ifndef IRRIGATION_CONFIG_RECORD_HPP
#define IRRIGATION_CONFIG_RECORD_HPP

#include <chrono>
#include "open_greenery/gpio/PinId.hpp"
#include "SensorReader.hpp"

namespace open_greenery::database
{

struct IrrigationConfigRecord
{
    open_greenery::gpio::PinId pin;
    std::int16_t dry;
    std::int16_t wet;
    std::uint16_t watering_volume;
    std::chrono::seconds watering_period;
    open_greenery::database::SensorReader soil_moisture_reader;
};

}

#endif //IRRIGATION_CONFIG_RECORD_HPP
