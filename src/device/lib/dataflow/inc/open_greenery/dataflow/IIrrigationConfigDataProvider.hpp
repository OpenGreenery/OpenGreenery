#ifndef I_IRRIGATION_CONFIG_DATA_PROVIDER_HPP
#define I_IRRIGATION_CONFIG_DATA_PROVIDER_HPP

#include <vector>
#include <QDateTime>
#include "IrrigationConfigRecord.hpp"

namespace open_greenery::dataflow
{

class IIrrigationConfigDataProvider
{
public:
    virtual std::vector<IrrigationConfigRecord> read() = 0;
    virtual IrrigationConfigRecord read(open_greenery::gpio::PinId _pin) = 0;

    IIrrigationConfigDataProvider() = default;
    virtual ~IIrrigationConfigDataProvider() = default;
    IIrrigationConfigDataProvider(const IIrrigationConfigDataProvider &) = delete;
    IIrrigationConfigDataProvider & operator=(const IIrrigationConfigDataProvider &) = delete;
};

}

#endif //I_IRRIGATION_CONFIG_DATA_PROVIDER_HPP
