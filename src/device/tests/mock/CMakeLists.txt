project(og-mock)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
        lib/dataflow
        lib/relay
)
target_link_libraries(${PROJECT_NAME} INTERFACE
        gmock
        dataflow
        relay
)
