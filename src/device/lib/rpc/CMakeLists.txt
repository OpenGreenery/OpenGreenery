project(rpc)

add_subdirectory(relay)

add_library(rpc INTERFACE)
target_link_libraries(rpc INTERFACE rpc_relay)
