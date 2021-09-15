#include <open_greenery/irrigation/moisture/MoistureModeHandler.hpp>
#include <SQLiteCpp/Exception.h>
#include <iostream>

namespace open_greenery::irrigation::moisture
{

MoistureModeHandler::MoistureModeHandler(const open_greenery::dataflow::irrigation::IrrigationConfigRecord & config,
                                         std::shared_ptr<open_greenery::dataflow::irrigation::ISensorReadProvider> sensor_reader,
                                         std::shared_ptr<open_greenery::pump::IPump> pump)
         :m_sensor_reader(std::move(sensor_reader)),
         m_state_machine_context(std::make_unique<Context>(config, std::move(pump)))
{}

void MoistureModeHandler::handle()
{
    std::int16_t soil_moisture {};
    try
    {
        soil_moisture = m_sensor_reader->get();
    }
    catch (const SQLite::Exception & _ex)
    {
        std::cerr << "Irrigation controller: SQLite error: " << _ex.getErrorCode() << " "
                  << _ex.getExtendedErrorCode() << " " << _ex.getErrorStr() << " " << _ex.what() << std::endl;
        return;
    }

    m_state_machine_context->handleSoilMoisture(soil_moisture);
}

}
