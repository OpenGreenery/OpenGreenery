project( analog_sensor )

set( SOURCES    src/AnalogSensor.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}    adc )
