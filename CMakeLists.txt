cmake_minimum_required(VERSION 3.9)
project(tcp_server VERSION 1.0.0 DESCRIPTION "tcp server lib. Preforced TCP server")

########################################################################
# tcp_server lib
########################################################################
file(GLOB_RECURSE SOURCES "src/*.c")
add_library(tcp_server SHARED ${SOURCES})

set_target_properties(tcp_server PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/tcp_server.h)

target_include_directories(tcp_server PRIVATE src/)
target_include_directories(tcp_server PUBLIC include/)

########################################################################
# libraries
########################################################################
add_subdirectory(libs/log)
add_subdirectory(libs/utils)

########################################################################
# executables
########################################################################


target_link_libraries(tcp_server LINK_PUBLIC log)
target_link_libraries(tcp_server LINK_PUBLIC utils)


add_executable(
    tcp_server_selftest
    "src/tcp_server_selftest.c"
)

target_include_directories(tcp_server_selftest PRIVATE src/)
target_include_directories(tcp_server_selftest PUBLIC include/)

target_include_directories(tcp_server_selftest PUBLIC libs/log/include)
target_include_directories(tcp_server_selftest PUBLIC libs/utils/include)
if (TARGET tcp_server)

target_link_libraries(tcp_server_selftest LINK_PRIVATE tcp_server)
target_link_libraries(tcp_server_selftest LINK_PUBLIC log)
target_link_libraries(tcp_server_selftest LINK_PUBLIC utils)

endif()


install(TARGETS tcp_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


install(TARGETS tcp_server DESTINATION ${CMAKE_INSTALL_BINDIR})
