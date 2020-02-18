#include <iostream>
#include <string>
#include <thread>
#include "open_greenery/sensor/AnalogSensor.hpp"
#include "open_greenery/adc/ADCFactory.hpp"
#include "SQLiteCpp/SQLiteCpp.h"

namespace og = open_greenery;

int main ()
{
    auto reader = og::adc::ADCFactory::getInstance().getReader(
            og::driver::ADS1115::Address::GND,
            og::driver::ADS1115::MUX::SINGLE_0);
    auto delay = std::chrono::milliseconds(500);

    og::sensor::AnalogSensor a0(reader, delay);

    SQLite::Database db("/tmp/sensor_data.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    std::cout << "SQLite database file " << db.getFilename() << " opened successfully" << std::endl;

    std::string dbName ("A0");
    db.exec("DROP TABLE IF EXISTS "+dbName);
    db.exec("CREATE TABLE "+dbName+" (id INTEGER PRIMARY KEY, value INTEGER)");

    std::uint16_t id {0};
    og::sensor::AnalogSensor::Notificator ntr = [&id, &db, &dbName](const std::uint16_t _val)
    {
        try
        {
            // Begin transaction
            SQLite::Transaction transaction(db);

            std::string id_s = std::to_string(id++);
            std::string val_s = std::to_string(_val);
            int rv = db.exec("INSERT INTO "+dbName+" VALUES ("+id_s+", "+val_s+")");
            std::cout << "INSERT INTO "+dbName+" VALUES ("+id_s+", "+val_s+"): " << rv << std::endl;

            // Commit transaction
            transaction.commit();
        }
        catch (std::exception & e)
        {
            std::cout << "SQLite exception: " << e.what() << std::endl;
        }
    };
    a0.subscribe(ntr);
    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
