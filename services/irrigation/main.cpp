#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <open_greenery/database/IrrigationConfigReader.hpp>
#include <open_greenery/pump/Pump.hpp>
#include <open_greenery/irrigation/DryState.hpp>
#include <open_greenery/irrigation/WetState.hpp>

constexpr open_greenery::gpio::PinId PUMP_PIN {0, open_greenery::gpio::Pinout::WIRING_PI};
namespace ogirig = open_greenery::irrigation;
using namespace std::chrono_literals;

int main()
{
    auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/open_greenery.db3", SQLite::OPEN_READONLY);
    std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;

    open_greenery::database::IrrigationConfigReader cfg_reader ({db, "IrrigationConfig"});
    auto config = cfg_reader.read(PUMP_PIN);
    std::cout << "Pump pin: " << config.pin.pin << std::endl
            << "Dry level: " << config.dry << " Wet level: " << config.wet << std::endl
            << "Watering volume: " << config.watering_volume << " ml" << std::endl
            << "Watering period: " << config.watering_period.count() << std::endl
            << "Soil moisture sensor: " << config.soil_moisture_reader.table().name << std::endl;

    auto UpdateMoisture = [](open_greenery::database::SensorReader _reader,
            std::atomic_int16_t & _moisture,
            std::chrono::seconds _period)
    {
        while (true)
        {
            try
            {
                _moisture = _reader.readLast().value;
            }
            catch (const SQLite::Exception & _ex)
            {
                std::cerr << "Soil Moisture: SQLite error: " << _ex.getErrorCode() << " "
                    << _ex.getExtendedErrorCode() << " " << _ex.getErrorStr() << " " << _ex.what() << std::endl;
            }
            std::cout << "Soil Moisture: " << _moisture << std::endl;
            std::this_thread::sleep_for(_period);
        }
    };


    std::atomic_int16_t current_soil_moisture {};
    std::cout << "Irrigation Controller: prepared memory for moisture data" << std::endl;
    std::thread soil_moisture_update_thr (
            UpdateMoisture,
            config.soil_moisture_reader,
            std::ref(current_soil_moisture),
            1s
        );
    soil_moisture_update_thr.detach();
    std::cout << "Irrigation Controller: started moisture updating thread" << std::endl;

    std::cout << "Irrigation Controller: create pump on pin=" << PUMP_PIN.pin << std::endl;
    open_greenery::pump::Pump pump (PUMP_PIN);

    std::cout << "Irrigation Controller: prepare state machine" << std::endl;
    using StatesStorage = std::map<ogirig::SystemState, std::shared_ptr<ogirig::State>>;
    StatesStorage state_machine {
            {ogirig::SystemState::WET, std::make_shared<ogirig::WetState>(config)},
            {ogirig::SystemState::DRY, std::make_shared<ogirig::DryState>(config, pump)}
    };

    auto to_str = [](ogirig::SystemState _s){return _s==ogirig::SystemState::WET?"WET":"DRY";};

    auto current_state = state_machine.find(ogirig::SystemState::WET)->second;
    std::cout << "Irrigation Controller: start main loop" << std::endl;
    while (true)
    {
        ogirig::SystemState next_state {current_state->handleSoilMoisture(current_soil_moisture)};
        std::cout << "Irrigation Controller: " << to_str(current_state->state()) << " -> " << to_str(next_state) << std::endl;
        if (next_state != current_state->state())
        {
            current_state = state_machine.find(next_state)->second;
        }
    }

    return 0;
}