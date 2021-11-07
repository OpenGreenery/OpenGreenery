project( og-ventilation )

add_executable( ${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
        og-relay_impl
        rpc)
