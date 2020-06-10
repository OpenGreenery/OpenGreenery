#include <open_greenery/database/IrrigationConfigReader.hpp>

namespace open_greenery::database
{

IrrigationConfigReader::IrrigationConfigReader(Table _table)
        : DatabaseEntity(std::move(_table))
{}

std::vector<IrrigationConfigRecord> IrrigationConfigReader::read()
{
    return read("SELECT * FROM "+table().name);
}

IrrigationConfigRecord IrrigationConfigReader::read(open_greenery::gpio::PinId _pin)
{
    const auto pin = std::to_string(_pin.cast_to(open_greenery::gpio::Pinout::WIRING_PI).pin);
    const auto db_data = read("SELECT * FROM "+table().name+" WHERE pump_pin == "+pin);

    if (db_data.empty())
    {
        throw std::runtime_error("There is no configuration for pump on pin "+pin);
    }
    if (db_data.size() > 1)
    {
        throw std::runtime_error("There are more than one configuration for pump on "+pin);
    }

    return db_data.front();
}

std::vector<IrrigationConfigRecord> IrrigationConfigReader::read(const std::string & _query)
{
    SQLite::Statement query {*table().database, _query};
    std::vector<IrrigationConfigRecord> rv;
    while (query.executeStep())
    {
        const char * pump_pin = query.getColumn("pump_pin");
        const char * dry_level = query.getColumn("dry_level");
        const char * wet_level = query.getColumn("wet_level");
        const char * watering_volume_ml = query.getColumn("watering_volume_ml");
        const char * watering_period_sec = query.getColumn("watering_period_sec");
        const char * soil_moisture_sensor = query.getColumn("soil_moisture_sensor");

        auto to_uint = [](const char * _str){return std::strtoul(_str, nullptr, 10);};
        auto to_int = [](const char * _str){return std::strtol(_str, nullptr, 10);};

        IrrigationConfigRecord rec = {
                {std::uint8_t(to_uint(pump_pin)), open_greenery::gpio::Pinout::WIRING_PI},
                std::int16_t(to_int(dry_level)),
                std::int16_t(to_int(wet_level)),
                std::uint16_t(to_uint(watering_volume_ml)),
                std::chrono::seconds(to_uint(watering_period_sec)),
                {{table().database, soil_moisture_sensor}}
        };
        rv.push_back(std::move(rec));
    }

    rv.shrink_to_fit();
    return rv;
}

}
