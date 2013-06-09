set (INCLUDES magnet.h btih.h)
set (SOURCES  magnet.c btih.c)

add_subdirectory (utils)
add_subdirectory (magnet)

if (LIBBTR_SHARED MATCHES true)
    add_library (${LIBBTR_TARGET} SHARED ${SOURCES})
    target_link_libraries (${LIBBTR_TARGET} talloc2 m)
endif ()

if (LIBBTR_STATIC MATCHES true)
    add_library (${LIBBTR_TARGET}_static STATIC ${SOURCES})
    set_target_properties (${LIBBTR_TARGET}_static PROPERTIES OUTPUT_NAME ${LIBBTR_TARGET})
    target_link_libraries (${LIBBTR_TARGET}_static talloc2_static m)
endif ()