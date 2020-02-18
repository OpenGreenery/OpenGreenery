#include <iostream>
#include <string>
#include <thread>
#include "open_greenery/adc/ADCFactory.hpp"
#include "open_greenery/database/SensorWriter.hpp"
#include "open_greenery/sensor/AnalogSensor.hpp"

namespace og = open_greenery;

int main ()
{
    // ADS1115 A0 port reader
    auto reader = og::adc::ADCFactory::getInstance().getReader(
            og::driver::ADS1115::Address::GND,
            og::driver::ADS1115::MUX::SINGLE_0);
    auto period = std::chrono::milliseconds(330);

    // Publisher of ADC readers data each 0.5 sec
    og::sensor::AnalogSensor a0(reader, period);

    // Open database file
    auto db = std::make_shared<SQLite::Database>("/tmp/sensor_data.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;

    std::string tableName ("A0");// Name of database table for sensor data
    og::database::SensorWriter writer(db, tableName);// Database sensor writer saves data to the table

    // Notificator that saves sensor data to the database
    og::sensor::AnalogSensor::Notificator ntr = [&writer](const std::uint16_t _val)
    {
        try
        {
            writer.write(_val);
            std::cout << "ADC value [" << _val << "] successfully saved" << std::endl;
        }
        catch (std::exception & e)
        {
            std::cout << "SQLite exception: " << e.what() << std::endl;
        }
    };
    a0.subscribe(ntr);// Transfer notificator to the A0 sensor
    // Pause main thread; Sensor publisher continues save ADC data via SensorWriter in the Notificator
    std::this_thread::sleep_for(std::chrono::seconds(20));
    // Finish main thread and sensor publisher
    return 0;
}
