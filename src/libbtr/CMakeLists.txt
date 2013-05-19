set (INCLUDES magnet.h btih.h)
set (SOURCES  magnet.c btih.c)

add_subdirectory (utils)
add_subdirectory (magnet)

add_library (libbtr SHARED ${SOURCES})
target_link_libraries (libbtr talloc m)