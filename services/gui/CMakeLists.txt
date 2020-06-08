project(gui)

set(CMAKE_AUTOMOC ON)
find_package( Qt5 REQUIRED COMPONENTS    Core
                                         Widgets
                                         Charts )

add_library( ${PROJECT_NAME}_impl    src/MainWindow.cpp
                                     src/Topbar.cpp
                                     src/page/DateValueChart.cpp
                                     src/page/MultipleCharts.cpp )
target_include_directories( ${PROJECT_NAME}_impl    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}_impl    ${LIBC}
                                               database
                                               analog_sensor
                                               Qt5::Core
                                               Qt5::Widgets
                                               Qt5::Charts )

add_executable( ${PROJECT_NAME}    main.cpp )
target_link_libraries( ${PROJECT_NAME}    ${PROJECT_NAME}_impl )
