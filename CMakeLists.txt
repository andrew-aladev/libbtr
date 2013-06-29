cmake_minimum_required (VERSION 2.8)
project (libbtr)

set (BOOLEANS LIBBTR_SHARED LIBBTR_STATIC)
foreach (bool ${BOOLEANS})
    if (NOT DEFINED ${bool})
        set (${bool} true)
    endif ()
endforeach()

if (NOT DEFINED LIBBTR_TARGET)
    set (LIBBTR_TARGET ${PROJECT_NAME})
endif ()

set (CMAKE_BUILD_TYPE DEBUG)

set (TALLOC_TARGET libbtr_talloc2)
add_subdirectory (talloc2)
include_directories ("talloc2/src/")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Winline -std=gnu99")
set (CMAKE_C_FLAGS_DEBUG    "-O0 -g")

set (CMAKE_C_FLAGS_RELEASE  "-O2 -flto")
set (CMAKE_LD_FLAGS_RELEASE "-flto")

add_subdirectory (src)

include_directories ("src/")

enable_testing ()
add_subdirectory (tests)
