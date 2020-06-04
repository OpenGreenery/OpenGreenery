project( database )

set( SOURCES    src/SensorReader.cpp
                src/SensorWriter.cpp
                src/Table.cpp)

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )

find_package( Qt5 REQUIRED COMPONENTS    Core )
target_link_libraries( ${PROJECT_NAME}    SQLiteCpp
                                          Qt5::Core )
