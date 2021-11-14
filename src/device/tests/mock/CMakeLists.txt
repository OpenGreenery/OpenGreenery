project(og-mock)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
        lib/dataflow
        lib/gpio
        lib/relay
)
target_link_libraries(${PROJECT_NAME} INTERFACE
        gmock
        gpio
        dataflow
        relay
)
