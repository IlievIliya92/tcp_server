# Setting this is required
cmake_minimum_required(VERSION 2.8)
# Project name
project(server)

# Enable verbose output during compile & linking
set(CMAKE_VERBOSE_MAKEFILE ON)

## Compile Flags ###############################################################
set(CMAKE_C_STANDARD 11)

## Add compoments ##############################################################
add_subdirectory(log)
add_subdirectory(utils)
add_subdirectory(tcp_server)
