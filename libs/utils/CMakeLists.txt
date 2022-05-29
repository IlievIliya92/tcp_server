cmake_minimum_required(VERSION 3.9)
project(utils VERSION 1.0.0 DESCRIPTION "utils lib. General purpouse utils")

file(GLOB_RECURSE SOURCES "src/*.c")
add_library(utils SHARED ${SOURCES})

set_target_properties(utils PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/utils.h)

target_include_directories(utils PRIVATE src/)
target_include_directories(utils PUBLIC include/)

#install(TARGETS utils
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
