project(rpc)

# gRPC
include(grpc.cmake)

# Proto file
get_filename_component(greeter_proto "protos/greeter.proto" ABSOLUTE)
get_filename_component(greeter_proto_path "${greeter_proto}" PATH)

set(PROTOC_GEN ${CMAKE_CURRENT_BINARY_DIR}/protoc_generated)
file(MAKE_DIRECTORY ${PROTOC_GEN})  # create directory for the generated files

set(greeter_proto_srcs "${PROTOC_GEN}/greeter.pb.cc")
set(greeter_proto_hdrs "${PROTOC_GEN}/greeter.pb.h")
set(greeter_grpc_srcs "${PROTOC_GEN}/greeter.grpc.pb.cc")
set(greeter_grpc_hdrs "${PROTOC_GEN}/greeter.grpc.pb.h")

add_custom_command(
        OUTPUT "${greeter_proto_srcs}" "${greeter_proto_hdrs}" "${greeter_grpc_srcs}" "${greeter_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${PROTOC_GEN}"
        --cpp_out "${PROTOC_GEN}"
        -I "${greeter_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${greeter_proto}"
        DEPENDS "${greeter_proto}")

# greeter_grpc_proto
add_library(greeter_grpc_proto
        ${greeter_grpc_srcs}
        ${greeter_grpc_hdrs}
        ${greeter_proto_srcs}
        ${greeter_proto_hdrs})
target_include_directories(greeter_grpc_proto PUBLIC inc ${PROTOC_GEN})
target_link_libraries(greeter_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})


find_package(gRPC CONFIG REQUIRED)
# /gRPC

#add_executable(client greeter_client.cpp)
#target_link_libraries(client PRIVATE
#        greeter_grpc_proto
#        ${_REFLECTION}
#        ${_GRPC_GRPCPP}
#        ${_PROTOBUF_LIBPROTOBUF})