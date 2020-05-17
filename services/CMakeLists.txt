project(services)

add_executable( gui gui.cpp )
find_package( Qt5 REQUIRED COMPONENTS    Core
                                         Widgets
                                         Charts )
target_link_libraries( gui    ${LIBC}
                              database
                              Qt5::Core
                              Qt5::Widgets
                              Qt5::Charts )
