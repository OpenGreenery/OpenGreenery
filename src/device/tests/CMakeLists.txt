project( tests )

add_subdirectory(mock)

enable_testing()

set( SRCS    main.cpp
             integration/relay-controller_rpc-lib/RelayRpcTest.cpp
             lib/database/DatabaseTest.cpp
             lib/database/light/LigthDaoTest.cpp
             lib/pump/PumpTest.cpp
             lib/relay/RelayTest.cpp
             lib/rpc/relay/RelayRPCTest.cpp
             services/relay/RelayControllerTest.cpp
)

add_executable( ${PROJECT_NAME}    ${SRCS} )
target_link_libraries( ${PROJECT_NAME}
        gtest
        og-mock
        database
        dataflow
        pump
        relay
        rpc
        og-relay_impl
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
