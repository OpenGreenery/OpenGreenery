# Define our host system
set( CMAKE_SYSTEM_NAME    Linux )
set( CMAKE_SYSTEM_VERSION    1 )

# Path to cross compilation Qt configurations
set( CMAKE_PREFIX_PATH    /usr/lib/arm-linux-gnueabihf/cmake )

set( CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra" )
set( CMAKE_CXX_STANDARD_REQUIRED    ON )
