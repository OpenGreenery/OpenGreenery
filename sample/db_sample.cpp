#include <iostream>
#include <string>
#include <thread>
#include <QDebug>
#include "open_greenery/adc/ADCFactory.hpp"
#include "open_greenery/database/SensorReader.hpp"
#include "open_greenery/database/SensorWriter.hpp"
#include "open_greenery/sensor/AnalogSensor.hpp"

namespace og = open_greenery;

int main ()
{
    // ADS1115 A0 port reader
    auto adc_reader = og::adc::ADCFactory::getInstance().getReader(
            og::driver::ADS1115::Address::GND,
            og::driver::ADS1115::MUX::SINGLE_0);
    constexpr auto period = std::chrono::milliseconds(330);

    // Publisher of ADC readers data each 0.5 sec
    og::sensor::AnalogSensor a0(adc_reader, period);

    // Open database file
    auto db = std::make_shared<SQLite::Database>("/tmp/sensor_data.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;

    const og::database::Table table (db, "A0");// Declare the database table for sensor data
    og::database::SensorWriter writer(table);// Database sensor writer saves data to the table

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
    const auto from = QDateTime::currentDateTime();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    const auto to = QDateTime::currentDateTime();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    a0.disable();

    og::database::SensorReader reader (table);
    std::vector<og::database::SensorData> res = reader.read(from, to);
    for (const auto pair : res)
    {
        const auto [dt, val] = pair;
        qDebug() << dt << " | " << val;
    }

    return 0;
}
