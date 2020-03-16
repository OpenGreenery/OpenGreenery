cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    rpi-toolchain.cmake )# Cross compile toolchain
set ( CMAKE_CXX_STANDARD 14 )
project( open_greenery )

add_subdirectory( lib )
add_subdirectory( sample )

add_executable( soil_moisture    soil_moisture.cpp )
target_link_libraries( soil_moisture    analog_sensor
                                        database
                                        spdlog )