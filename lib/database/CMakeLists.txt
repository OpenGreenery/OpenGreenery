project( database )

set( SOURCES    src/DatabaseEntity.cpp
                src/IrrigationConfigReader.cpp
                src/IrrigationConfigWriter.cpp
                src/SensorReader.cpp
                src/SensorWriter.cpp
                src/Table.cpp)

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc
                                               PUBLIC ${LIBS}/gpio/inc )
find_package( Qt5 REQUIRED COMPONENTS    Core )
target_link_libraries( ${PROJECT_NAME}    dataflow
                                          SQLiteCpp
                                          Qt5::Core )
