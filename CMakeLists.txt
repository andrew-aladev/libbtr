cmake_minimum_required(VERSION 2.8)
project(btbot)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=gnu99 -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")

add_subdirectory(src)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/btbot")

add_subdirectory(tests)
add_subdirectory(examples)