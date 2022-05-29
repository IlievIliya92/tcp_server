cmake_minimum_required(VERSION 3.9)
project(log VERSION 1.0.0 DESCRIPTION "log lib.")

file(GLOB_RECURSE SOURCES "src/*.c")
add_library(log SHARED ${SOURCES})

set_target_properties(log PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/log.h)

target_include_directories(log PRIVATE src/)
target_include_directories(log PUBLIC include/)

#install(TARGETS log
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
