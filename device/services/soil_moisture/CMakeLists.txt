project( services )

add_executable( soil_moisture    soil_moisture.cpp )
target_link_libraries( soil_moisture    analog_sensor
                                        database )
