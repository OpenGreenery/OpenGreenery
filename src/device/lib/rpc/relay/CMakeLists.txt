project(rpc_relay)

include(../grpc.cmake)

# Proto file
get_filename_component(relay_proto "protos/relay.proto" ABSOLUTE)
get_filename_component(relay_proto_path "${relay_proto}" PATH)

# Directory for generated files
set(PROTOC_GEN ${CMAKE_CURRENT_BINARY_DIR}/protoc_generated)
file(MAKE_DIRECTORY ${PROTOC_GEN})  # create directory for the generated files

# Generated files
set(relay_proto_srcs "${PROTOC_GEN}/relay.pb.cc")
set(relay_proto_hdrs "${PROTOC_GEN}/relay.pb.h")
set(relay_grpc_srcs "${PROTOC_GEN}/relay.grpc.pb.cc")
set(relay_grpc_hdrs "${PROTOC_GEN}/relay.grpc.pb.h")

# Generate files
add_custom_command(
        OUTPUT "${relay_proto_srcs}" "${relay_proto_hdrs}" "${relay_grpc_srcs}" "${relay_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${PROTOC_GEN}"
        --cpp_out "${PROTOC_GEN}"
        -I "${relay_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${relay_proto}"
        DEPENDS "${relay_proto}")

# Wrappers implementation
set(SRCS
        src/Client.cpp
        src/Server.cpp)
add_library(${PROJECT_NAME}
        ${relay_grpc_srcs}
        ${relay_grpc_hdrs}
        ${relay_proto_srcs}
        ${relay_proto_hdrs}
        ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC inc ${PROTOC_GEN})
target_link_libraries(${PROJECT_NAME}
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        spdlog
        dataflow)
