cmake_minimum_required (VERSION 2.8)
project (libbtr)

set (CMAKE_VERBOSE_MAKEFILE false)
set (CMAKE_BUILD_TYPE Debug)
enable_testing ()

add_subdirectory (talloc)
include_directories ("talloc/src/")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Winline -std=gnu99")
set (CMAKE_C_FLAGS_DEBUG "-O2 -g")
add_subdirectory (src)

include_directories ("src/")

add_subdirectory (tests)
# add_subdirectory (examples)