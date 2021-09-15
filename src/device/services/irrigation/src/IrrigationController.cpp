#include <utility>
#include <iostream>
#include <SQLiteCpp/Exception.h>
#include <open_greenery/irrigation/IrrigationController.hpp>

namespace open_greenery::irrigation
{

IrrigationController::IrrigationController(open_greenery::dataflow::irrigation::IrrigationConfigRecord _cfg,
                                           std::shared_ptr<open_greenery::dataflow::irrigation::ISensorReadProvider> _sensor_reader,
                                           std::shared_ptr<open_greenery::pump::IPump> _pump)
    :m_cfg(std::move(_cfg)),

    m_moisture_mode_handler(_cfg, std::move(_sensor_reader), std::move(_pump))
{}

IrrigationController::~IrrigationController()
{
    stop();
}

open_greenery::tools::FinishFuture IrrigationController::start()
{
    if (!m_irrigation_thr)
    {
        m_irrigation_thr = std::make_unique<open_greenery::tools::LoopThread>(
                std::bind(&IrrigationController::IrrigationThreadFunc, this), m_cfg.watering_period
            );
    }
    return m_irrigation_thr->start();
}

void IrrigationController::stop()
{
    if (!m_irrigation_thr)
    {
        return;
    }

    m_irrigation_thr->stop();
    m_irrigation_thr.reset();
}

void IrrigationController::IrrigationThreadFunc()
{
    m_moisture_mode_handler.handle();
}

}
