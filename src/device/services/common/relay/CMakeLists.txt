project( og-relay )

set( SRCS   src/RelayController.cpp
            src/CurrentTimeProvider.cpp )

add_library( ${PROJECT_NAME}_impl    ${SRCS} )
target_include_directories( ${PROJECT_NAME}_impl    PUBLIC inc )
find_package( Qt5 REQUIRED COMPONENTS    Core )
target_link_libraries( ${PROJECT_NAME}_impl    relay
                                               dataflow
                                               tools
                                               Qt5::Core
                                               spdlog )
