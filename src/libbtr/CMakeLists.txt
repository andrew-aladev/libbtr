set (INCLUDES magnet.h btih.h)
set (SOURCES  magnet.c btih.c)

add_subdirectory (utils)
add_subdirectory (magnet)

if (LIBBTR_SHARED MATCHES true)
    add_library (libbtr SHARED ${SOURCES})
    target_link_libraries (libbtr talloc m)
endif ()

if (LIBBTR_STATIC MATCHES true)
    add_library (libbtr_static STATIC ${SOURCES})
    set_target_properties (libbtr_static PROPERTIES OUTPUT_NAME libbtr)
    target_link_libraries (libbtr_static talloc_static m)
endif ()