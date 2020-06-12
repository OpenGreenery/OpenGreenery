project( irrigation )

set( SRCS    src/DryState.cpp
             src/State.cpp
             src/WetState.cpp )

add_library( ${PROJECT_NAME}_impl    ${SRCS} )
target_include_directories( ${PROJECT_NAME}_impl    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}_impl    database
                                               pump )

add_executable( ${PROJECT_NAME}    main.cpp )
target_link_libraries( ${PROJECT_NAME}    ${PROJECT_NAME}_impl )
