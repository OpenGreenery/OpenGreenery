# :herb: OpenGreenery
[![Device Software](https://github.com/OpenGreenery/OpenGreenery/actions/workflows/device-sw.yml/badge.svg)](https://github.com/OpenGreenery/OpenGreenery/actions/workflows/device-sw.yml) [![Android Application](https://github.com/OpenGreenery/OpenGreenery/actions/workflows/android.yml/badge.svg)](https://github.com/OpenGreenery/OpenGreenery/actions/workflows/android.yml)

Plants growing automated system

## Features
- Lights and ventilation control
  - Manually by user
  - By time of day
- Irrigation
  - Periodically
  - Soil moisture based

## Hardware
- Raspberry Pi 3B+
- ADC ADS1115
- 3500K LED phytolamp
- Duct fan
- Soil moisture sensors
- Peristaltic pump
- Solenoid valves
- Relays
- PSU 12V 10A
- 12V to 5V DC-DC converter

## Software stack
### Device services
- C++17
- CMake
- gRPC
- SQLite
- GoogleTest
### Android application
- Kotlin
- gRPC
