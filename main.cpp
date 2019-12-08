#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <wiringPi.h>

int main ()
{
    std::cout << "Hello OpenGreener!" << std::endl;
    
    // Toggle pin sample
    constexpr std::uint16_t PIN {0u}, TIMES {3u};
    wiringPiSetup();
    pinMode(PIN, OUTPUT);
    for (std::uint8_t i {0u}; i < TIMES; ++i)
    {
        std::cout << "Enable pin " << PIN << std::endl;
        digitalWrite(PIN, HIGH);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Disable pin " << PIN << std::endl;
        digitalWrite(PIN, LOW);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    pinMode(PIN, INPUT);

    return 0;
}
