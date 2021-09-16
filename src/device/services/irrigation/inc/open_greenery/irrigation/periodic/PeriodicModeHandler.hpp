#pragma once

#include <memory>
#include <optional>
#include <open_greenery/dataflow/irrigation/Participants.hpp>
#include <open_greenery/dataflow/time/Participants.hpp>
#include <open_greenery/pump/IPump.hpp>

namespace open_greenery::irrigation::periodic
{

class PeriodicModeHandler
{
public:
    PeriodicModeHandler(
            std::unique_ptr<open_greenery::dataflow::irrigation::IPeriodicModeConfigProvider> config_provider,
            std::shared_ptr<open_greenery::dataflow::time::ICurrentDateTimeProvider> datetime_provider,
            std::shared_ptr<open_greenery::pump::IPump> pump
    );

    void handle();

private:
    std::unique_ptr<open_greenery::dataflow::irrigation::IPeriodicModeConfigProvider> m_config_provider;
    std::shared_ptr<open_greenery::dataflow::time::ICurrentDateTimeProvider> m_datetime_provider;
    std::shared_ptr<open_greenery::pump::IPump> m_pump;

    std::optional<open_greenery::dataflow::irrigation::PeriodicModeConfig> m_current_config;
    QDateTime m_next_irrigation_tp;
};

}
