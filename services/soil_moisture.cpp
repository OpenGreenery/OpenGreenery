#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "open_greenery/adc/ADCFactory.hpp"
#include "open_greenery/database/SensorWriter.hpp"
#include "open_greenery/sensor/AnalogSensor.hpp"

namespace og = open_greenery;

struct ChannelEntities
{
    std::shared_ptr<og::adc::IADCReader> adc_reader;
    std::unique_ptr<og::sensor::AnalogSensor> sensor;
    std::unique_ptr<og::database::SensorWriter> db_writer;
};

struct SampleData
{
    og::driver::ADS1115::MUX channel;
    std::string db_table_name;
};

int main ()
{
    try
    {
        const std::array<SampleData, 4> sample_data {{{og::driver::ADS1115::MUX::SINGLE_0, "A0"},
                                                      {og::driver::ADS1115::MUX::SINGLE_1, "A1"},
                                                      {og::driver::ADS1115::MUX::SINGLE_2, "A2"},
                                                      {og::driver::ADS1115::MUX::SINGLE_3, "A3"}}};
        std::array<ChannelEntities, 4> entities;
        og::adc::ADCFactory &adc_factory = og::adc::ADCFactory::getInstance();

        // Open database file
        auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/soil_moisture.db3",
                                                     SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;

        // Notificator that saves sensor data to the database
        auto make_notificator = [](og::database::SensorWriter &writer, std::string logline)
                -> og::sensor::AnalogSensor::Notificator
        {
            return [&writer, logline](const std::int16_t _val)
            {
                try
                {
                    writer.write(_val);
                    std::cout << logline << " = " << _val << std::endl;
                }
                catch (std::exception &e)
                {
                    std::cerr << "SQLite exception: " << e.what() << std::endl;
                }
            };
        };

        auto entity = entities.begin();
        auto entity_data = sample_data.begin();
        for (; entity != entities.end() && entity_data != sample_data.end(); entity++, entity_data++)
        {
            // ADS1115 address
            constexpr og::driver::ADS1115::Address adc_addr = og::driver::ADS1115::Address::GND;
            entity->adc_reader = adc_factory.getReader(adc_addr, entity_data->channel);

            // Sensor publisher calls notificators with ADC readers data each minute
            constexpr auto period = std::chrono::minutes(1);
            entity->sensor = std::make_unique<og::sensor::AnalogSensor>(entity->adc_reader, period);

            // Database sensor writer saves data to the table
            og::database::Table table (db, entity_data->db_table_name);
            entity->db_writer = std::make_unique<og::database::SensorWriter>(table);

            // Transfer notificator to the sensor publisher
            entity->sensor->subscribe(make_notificator(*(entity->db_writer), entity_data->db_table_name));
        }

        // Pause main thread; Sensor publisher continues save ADC data via SensorWriter in the Notificator
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::minutes(5));
        }
    }
    catch (const std::exception & _ex)
    {
        std::cerr << "std::exception has been thrown: " << _ex.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown exception has been thrown" << std::endl;
    }
    // Finish main thread and sensor publisher
    return 0;
}
