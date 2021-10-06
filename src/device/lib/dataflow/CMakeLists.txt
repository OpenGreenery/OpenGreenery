project( dataflow )

add_library( ${PROJECT_NAME}    INTERFACE )
target_include_directories( ${PROJECT_NAME}    INTERFACE inc )
find_package( Qt5 REQUIRED COMPONENTS    Core )
target_link_libraries( ${PROJECT_NAME}    INTERFACE Qt5::Core gpio)
