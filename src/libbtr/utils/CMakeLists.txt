set (INCLUDES ${INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/itoa.h
    ${CMAKE_CURRENT_SOURCE_DIR}/url.h
PARENT_SCOPE)
set (SOURCES ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/itoa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/url.c
PARENT_SCOPE)