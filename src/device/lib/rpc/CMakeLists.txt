project(rpc)

add_subdirectory(light)

add_library(rpc INTERFACE)
target_link_libraries(rpc INTERFACE rpc_light)
