#include <iostream>
#include <open_greenery/database/IrrigationConfigReader.hpp>
#include <open_greenery/database/IrrigationConfigWriter.hpp>
#include <open_greenery/gpio/GPIOFactory.hpp>

namespace og = open_greenery;

int main()
{
    std::cout << "Create/open DB" << std::endl;
    auto db = std::make_shared<SQLite::Database>("/tmp/irig.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << ((db != nullptr) ? "not nullptr":"nullptr") << std::endl;
    std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;
    og::database::IrrigationConfigWriter writer ({db, "IrrigationConfig"});

    open_greenery::gpio::PinId pin {2, og::gpio::Pinout::WIRING_PI};
    std::int16_t dry {13000};
    std::int16_t wet {5000};
    std::uint16_t watering_volume {10};
    std::chrono::seconds watering_period {10};
    open_greenery::database::SensorReader soil_moisture_reader ({db, "A0"});
    writer.write({pin, dry, wet, watering_volume, watering_period, soil_moisture_reader});

    og::database::IrrigationConfigReader reader ({db, "IrrigationConfig"});
    const auto record = reader.read(pin);
    std::cout << "Pin: " << record.pin.pin << " Dry: " << dry << " Wet: " << wet << " Vol.: " << watering_volume
        << " Period: " << watering_period.count() << " Reader: " << soil_moisture_reader.table().name;

    return 0;
}