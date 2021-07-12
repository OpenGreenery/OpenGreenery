project( pump )

set( SOURCES    src/Pump.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}    relay )
