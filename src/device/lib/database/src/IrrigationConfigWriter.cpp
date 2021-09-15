#include <open_greenery/database/IrrigationConfigWriter.hpp>
#include <string>

namespace open_greenery::database
{

IrrigationConfigWriter::IrrigationConfigWriter(Table _table)
        : DatabaseEntity(std::move(_table)) {
    const auto query = std::string("CREATE TABLE IF NOT EXISTS ")
                       + table().name
                       + " (pump_pin INTEGER PRIMARY KEY NOT NULL, "
                       + "dry_level INTEGER NOT NULL, "
                       + "wet_level INTEGER NOT NULL, "
                       + "watering_volume_ml INTEGER NOT NULL, "
                       + "watering_period_sec INTEGER NOT NULL, "
                       + "soil_moisture_sensor CHAR(2))";
    table().database->exec(query);
}

void IrrigationConfigWriter::set(open_greenery::dataflow::irrigation::IrrigationConfigRecord _record) {
    SQLite::Transaction transaction(*table().database);

    const auto query = std::string("INSERT INTO ")
                       + table().name
                       + " VALUES ("
                       + std::to_string(_record.pin.cast_to(open_greenery::gpio::Pinout::WIRING_PI).pin) + ", "
                       + std::to_string(_record.dry) + ", "
                       + std::to_string(_record.wet) + ", "
                       + std::to_string(_record.watering_volume) + ", "
                       + std::to_string(_record.watering_period.count()) + ", "
                       + "\"" + _record.soil_moisture_sensor + "\")";

    int rv = table().database->exec(query);
    if (rv != 1) {
        throw std::runtime_error(
                "Database Error: Insert to the " + table().name + " returned " + std::to_string(rv));
    }
    transaction.commit();
}

}
