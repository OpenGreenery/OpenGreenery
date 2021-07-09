project( gpio )

set( SOURCES    src/GPIOctl.cpp
                src/GPIOFactory.cpp
                src/OutputGPIOctl.cpp
                src/InputGPIOctl.cpp
                src/PinId.cpp
                src/Pinout.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}    wiringPi )
