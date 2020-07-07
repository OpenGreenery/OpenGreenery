cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/rpi-toolchain.cmake )
set ( CMAKE_C_STANDARD 11 )
project( wiringPi )

# Have CMake find our pthreads library within our toolchain (required for this library)
set( CMAKE_THREAD_PREFER_PTHREAD    TRUE )
find_package( Threads    REQUIRED )
if( NOT Threads_FOUND )
	message( SEND_ERROR    "Failed to find Threads" )
	return()
endif()

set( SOURCES WiringPi/wiringPi )
set( SRC_FILES    ${SOURCES}/piHiPri.c
				  ${SOURCES}/wiringPiSPI.c
				  ${SOURCES}/mcp23s17.c
				  ${SOURCES}/mcp23016.c
				  ${SOURCES}/max31855.c
				  ${SOURCES}/mcp4802.c
				  ${SOURCES}/wiringShift.c
				  ${SOURCES}/wiringPiI2C.c
				  ${SOURCES}/mcp3004.c
				  ${SOURCES}/htu21d.c
				  ${SOURCES}/softPwm.c
				  ${SOURCES}/mcp23s08.c
				  ${SOURCES}/sn3218.c
				  ${SOURCES}/rht03.c
				  ${SOURCES}/bmp180.c
				  ${SOURCES}/pseudoPins.c
				  ${SOURCES}/pcf8591.c
				  ${SOURCES}/wiringPi.c
				  ${SOURCES}/max5322.c
				  ${SOURCES}/mcp23008.c
				  ${SOURCES}/mcp3422.c
				  ${SOURCES}/mcp23017.c
				  ${SOURCES}/softTone.c
				  ${SOURCES}/ads1115.c
				  ${SOURCES}/mcp3002.c
				  ${SOURCES}/drcSerial.c
				  ${SOURCES}/ds18b20.c
				  ${SOURCES}/drcNet.c
				  ${SOURCES}/pcf8574.c
				  ${SOURCES}/wiringSerial.c
				  ${SOURCES}/piThread.c
				  ${SOURCES}/sr595.c )
add_library( ${PROJECT_NAME}    SHARED ${SRC_FILES})
target_include_directories( ${PROJECT_NAME}    PUBLIC ${SOURCES} )

# link the following required libraries:
# Threads, Math, Crypt, and RealTime
target_link_libraries( ${PROJECT_NAME}    ${CMAKE_THREAD_LIBS_INIT}
                                          crypt
                                          m
                                          rt )
