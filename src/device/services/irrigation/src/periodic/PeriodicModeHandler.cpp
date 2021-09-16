#include <open_greenery/irrigation/periodic/PeriodicModeHandler.hpp>

namespace open_greenery::irrigation::periodic
{

PeriodicModeHandler::PeriodicModeHandler(
        std::unique_ptr<open_greenery::dataflow::irrigation::IPeriodicModeConfigProvider> config_provider,
        std::shared_ptr<open_greenery::dataflow::time::ICurrentDateTimeProvider> datetime_provider,
        std::shared_ptr<open_greenery::pump::IPump> pump
        )
        :m_config_provider(std::move(config_provider)),
        m_datetime_provider(std::move(datetime_provider)),
        m_pump(std::move(pump))
{
    assert(m_config_provider);
    assert(m_datetime_provider);
    assert(m_pump);
}

void PeriodicModeHandler::handle()
{
    // Update config
    const auto cfg = m_config_provider->get();
    if (cfg.has_value())
    {
        m_current_config = cfg;
    }

    // Skip if config is empty
    if (!m_current_config.has_value())
    {
        return;
    }

    const auto now = m_datetime_provider->get();
    if (now > m_next_irrigation_tp)
    {
        m_pump->water(m_current_config->volume);
        m_next_irrigation_tp = m_next_irrigation_tp.addMSecs(m_current_config->duration.count());
    }
}

}
