project( tests )

find_package(GTest REQUIRED)
include_directories( ${GTEST_INCLUDE_DIRS} )
enable_testing()

set( SRCS    main.cpp
             lib/database/DatabaseTest.cpp
             lib/database/light/LigthDaoTest.cpp
             lib/relay/RelayTest.cpp
             services/light/mock/EmptyCurrentTimeProvider.cpp
             services/light/mock/EmptyCurrentTimeProvider.hpp
             services/light/mock/Relay.cpp
             services/light/mock/Relay.hpp
             services/light/LightTest.cpp )

add_executable( ${PROJECT_NAME}    ${SRCS} )
target_link_libraries( ${PROJECT_NAME}    ${GTEST_LIBRARIES}
                                          database
                                          dataflow
                                          relay
                                          og-light_impl )
