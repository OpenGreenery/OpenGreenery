#ifndef I_IRRIGATION_CONFIG_DATA_RECEIVER_HPP
#define I_IRRIGATION_CONFIG_DATA_RECEIVER_HPP

#include <vector>
#include <QDateTime>
#include "SensorRecord.hpp"

namespace open_greenery::dataflow
{

class IIrrigationDataReceiver
{
public:
    virtual void write(IrrigationConfigRecord _record) = 0;

    IIrrigationDataReceiver() = default;
    virtual ~IIrrigationDataReceiver() = default;
    IIrrigationDataReceiver(const IIrrigationDataReceiver &) = delete;
    IIrrigationDataReceiver & operator=(const IIrrigationDataReceiver &) = delete;
};

}

#endif //I_IRRIGATION_CONFIG_DATA_RECEIVER_HPP
