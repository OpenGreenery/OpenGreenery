#ifndef I_SENSOR_READ_PROVIDER_HPP
#define I_SENSOR_READ_PROVIDER_HPP

#include "SensorRecord.hpp"

namespace open_greenery::dataflow
{

    class ISensorReadProvider
    {
    public:
        virtual std::int16_t read() const = 0;

        ISensorReadProvider() = default;
        virtual ~ISensorReadProvider() = default;
        ISensorReadProvider(const ISensorReadProvider &) = delete;
        ISensorReadProvider & operator=(const ISensorReadProvider &) = delete;
    };

}

#endif //I_SENSOR_READ_PROVIDER_HPP
