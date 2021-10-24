project(rpc_light)

include(../grpc.cmake)

# Proto file
get_filename_component(light_proto "protos/light.proto" ABSOLUTE)
get_filename_component(light_proto_path "${light_proto}" PATH)

# Directory for generated files
set(PROTOC_GEN ${CMAKE_CURRENT_BINARY_DIR}/protoc_generated)
file(MAKE_DIRECTORY ${PROTOC_GEN})  # create directory for the generated files

# Generated files
set(light_proto_srcs "${PROTOC_GEN}/light.pb.cc")
set(light_proto_hdrs "${PROTOC_GEN}/light.pb.h")
set(light_grpc_srcs "${PROTOC_GEN}/light.grpc.pb.cc")
set(light_grpc_hdrs "${PROTOC_GEN}/light.grpc.pb.h")

# Generate files
add_custom_command(
        OUTPUT "${light_proto_srcs}" "${light_proto_hdrs}" "${light_grpc_srcs}" "${light_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${PROTOC_GEN}"
        --cpp_out "${PROTOC_GEN}"
        -I "${light_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${light_proto}"
        DEPENDS "${light_proto}")

# Wrappers implementation
set(SRCS
        src/LightProxyClient.cpp
        src/LightProxyServer.cpp)
add_library(rpc_light
        ${light_grpc_srcs}
        ${light_grpc_hdrs}
        ${light_proto_srcs}
        ${light_proto_hdrs}
        ${SRCS})
target_include_directories(rpc_light PUBLIC inc ${PROTOC_GEN})
target_link_libraries(rpc_light
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        spdlog
        dataflow)
