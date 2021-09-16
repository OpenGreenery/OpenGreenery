#pragma once

#include <cstdint>
#include <vector>
#include "PeriodicModeConfig.hpp"
#include "SensorRecord.hpp"
#include "IrrigationConfigRecord.hpp"
#include <open_greenery/dataflow/common/IOptionalProvider.hpp>
#include <open_greenery/dataflow/common/IProvider.hpp>
#include <open_greenery/dataflow/common/IReceiver.hpp>

namespace open_greenery::dataflow::irrigation
{

using IPeriodicModeConfigProvider = open_greenery::dataflow::common::IOptionalProvider<PeriodicModeConfig>;
using IPeriodicModeConfigReceiver = open_greenery::dataflow::common::IReceiver<PeriodicModeConfig>;

class IIrrigationConfigDataProvider
{
public:
    virtual std::vector<IrrigationConfigRecord> read() = 0;
    virtual IrrigationConfigRecord read(open_greenery::gpio::PinId _pin) = 0;

    IIrrigationConfigDataProvider() = default;
    virtual ~IIrrigationConfigDataProvider() = default;
};
using IIrrigationDataReceiver = open_greenery::dataflow::common::IReceiver<IrrigationConfigRecord>;

using IOptionalSensorReadProvider = open_greenery::dataflow::common::IOptionalProvider<std::int16_t>;
using ISensorReadProvider = open_greenery::dataflow::common::IProvider<std::int16_t>;

class ISensorDataProvider
{
public:
    virtual std::vector<SensorRecord> read(QDateTime _from, QDateTime _to) const = 0;

    ISensorDataProvider() = default;
    virtual ~ISensorDataProvider() = default;
};

class ISensorDataReceiver
{
public:
    virtual void write(std::int16_t _data) = 0;
    virtual void write(SensorRecord _record) = 0;
    virtual void write(QDateTime _timestamp, std::int16_t _data) = 0;

    ISensorDataReceiver() = default;
    virtual ~ISensorDataReceiver() = default;
};

}
