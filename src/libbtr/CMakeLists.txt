set (INCLUDES btih.h epoll.h)
set (SOURCES  btih.c)

add_subdirectory (magnet)
add_subdirectory (plug)
add_subdirectory (utils)

if (LIBBTR_SHARED MATCHES true)
    add_library (${LIBBTR_TARGET} SHARED ${SOURCES})
    target_link_libraries (${LIBBTR_TARGET} ${TALLOC_TARGET} m)
endif ()

if (LIBBTR_STATIC MATCHES true)
    add_library (${LIBBTR_TARGET}_static STATIC ${SOURCES})
    set_target_properties (${LIBBTR_TARGET}_static PROPERTIES OUTPUT_NAME ${LIBBTR_TARGET})
    target_link_libraries (${LIBBTR_TARGET}_static ${TALLOC_TARGET}_static m)
endif ()