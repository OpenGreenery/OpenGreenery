project(og-cli)

set(SRCS main.cpp)
add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} rpc)
