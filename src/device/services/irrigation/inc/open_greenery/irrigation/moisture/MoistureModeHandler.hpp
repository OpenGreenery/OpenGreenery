#pragma once

#include <memory>
#include <open_greenery/dataflow/irrigation/Participants.hpp>
#include <open_greenery/dataflow/irrigation/IrrigationConfigRecord.hpp>
#include <open_greenery/dataflow/time/Participants.hpp>
#include <open_greenery/pump/IPump.hpp>
#include "state_machine/Context.hpp"

namespace open_greenery::irrigation::moisture
{

class MoistureModeHandler
{
public:
    MoistureModeHandler(
            const open_greenery::dataflow::irrigation::IrrigationConfigRecord & config,
            std::shared_ptr<open_greenery::dataflow::irrigation::ISensorReadProvider> sensor_reader,
            std::shared_ptr<open_greenery::pump::IPump> pump
            );

    void handle();
private:
    std::shared_ptr<open_greenery::dataflow::irrigation::ISensorReadProvider> m_sensor_reader;
    std::unique_ptr<Context> m_state_machine_context;
};

}
