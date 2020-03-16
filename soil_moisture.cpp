#include <iostream>
#include <string>
#include <thread>
#include "open_greenery/adc/ADCFactory.hpp"
#include "open_greenery/database/SensorWriter.hpp"
#include "open_greenery/sensor/AnalogSensor.hpp"
#include "spdlog/spdlog.h"

namespace og = open_greenery;

int main ()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("ADCFactory::getInstance()");
    og::adc::ADCFactory & factory = og::adc::ADCFactory::getInstance();
    // ADS1115 address
    spdlog::debug("ADC addr");
    const og::driver::ADS1115::Address adc_addr = og::driver::ADS1115::Address::GND;
    spdlog::debug("make readers");
    auto a0_reader = factory.getReader(adc_addr, og::driver::ADS1115::MUX::SINGLE_0);
    auto a1_reader = factory.getReader(adc_addr, og::driver::ADS1115::MUX::SINGLE_1);
    auto a2_reader = factory.getReader(adc_addr, og::driver::ADS1115::MUX::SINGLE_2);
    auto a3_reader = factory.getReader(adc_addr, og::driver::ADS1115::MUX::SINGLE_3);
    auto period = std::chrono::minutes (1);

    spdlog::debug("make sensors publishers");
    // Publisher of ADC readers data each minute
    og::sensor::AnalogSensor a0(a0_reader, period);
    og::sensor::AnalogSensor a1(a1_reader, period);
    og::sensor::AnalogSensor a2(a2_reader, period);
    og::sensor::AnalogSensor a3(a3_reader, period);

    spdlog::debug("open DB");
    // Open database file
    auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/soil_moisture.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;

    spdlog::debug("make DB writers");
    og::database::SensorWriter a0_db_writer(db, "A0");// Database sensor writer saves data to the table
    og::database::SensorWriter a1_db_writer(db, "A1");// Database sensor writer saves data to the table
    og::database::SensorWriter a2_db_writer(db, "A2");// Database sensor writer saves data to the table
    og::database::SensorWriter a3_db_writer(db, "A3");// Database sensor writer saves data to the table

    // Notificator that saves sensor data to the database
    spdlog::debug("define make_not");
    auto make_notificator = [](og::database::SensorWriter & writer, std::string logline) -> og::sensor::AnalogSensor::Notificator
    {
        return [&writer, logline](const std::uint16_t _val)
        {
            try
            {
                writer.write(_val);
                spdlog::info("{} = {:5d}", logline, _val);
            }
            catch (std::exception & e)
            {
                spdlog::error("SQLite exception: {}", e.what());
            }
        };
    };

    spdlog::debug("subscribe");
    a0.subscribe(make_notificator(a0_db_writer, "A0"));// Transfer notificator to the A0 sensor
    a1.subscribe(make_notificator(a1_db_writer, "A1"));// Transfer notificator to the A1 sensor
    a2.subscribe(make_notificator(a2_db_writer, "A2"));// Transfer notificator to the A2 sensor
    a3.subscribe(make_notificator(a3_db_writer, "A3"));// Transfer notificator to the A3 sensor

    // Pause main thread; Sensor publisher continues save ADC data via SensorWriter in the Notificator
    spdlog::debug("start main-thr infinite loop");
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
    // Finish main thread and sensor publisher
    return 0;
}
