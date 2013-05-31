set (INCLUDES magnet.h btih.h)
set (SOURCES  magnet.c btih.c)

add_subdirectory (utils)
add_subdirectory (magnet)

if (LIBBTR_SHARED MATCHES true)
    add_library (${PROJECT_NAME} SHARED ${SOURCES})
    target_link_libraries (${PROJECT_NAME} talloc2 m)
endif ()

if (LIBBTR_STATIC MATCHES true)
    add_library (${PROJECT_NAME}_static STATIC ${SOURCES})
    set_target_properties (${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    target_link_libraries (${PROJECT_NAME}_static talloc2_static m)
endif ()