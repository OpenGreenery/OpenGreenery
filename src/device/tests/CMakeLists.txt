project( tests )

enable_testing()

set( SRCS    main.cpp
             lib/database/DatabaseTest.cpp
             lib/database/light/LigthDaoTest.cpp
             lib/relay/RelayTest.cpp
             lib/rpc/light/LightProxy.cpp
             services/light/mock/CurrentTimeProvider.cpp
             services/light/mock/OptionalProvider.hpp
             services/light/mock/Relay.cpp
             services/light/mock/StatusReceiver.cpp
             services/light/LightTest.cpp )

add_executable( ${PROJECT_NAME}    ${SRCS} )
target_link_libraries( ${PROJECT_NAME}    gtest
                                          gmock
                                          database
                                          dataflow
                                          relay
                                          rpc
                                          og-light_impl )
