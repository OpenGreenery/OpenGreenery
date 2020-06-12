project( adc )

set( SOURCES    src/ADCFactory.cpp
                src/ADS1115Reader.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}    ads1115 )
