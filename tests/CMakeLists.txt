set (ITOA   itoa/main.c)
set (URL    url/main.c)
set (BTIH   btih/main.c)
set (MAGNET magnet/main.c)

if (LIBBTR_SHARED MATCHES true)
    add_executable (${LIBBTR_TARGET}-itoa ${ITOA})
    target_link_libraries (${LIBBTR_TARGET}-itoa ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-itoa ${LIBBTR_TARGET}-itoa)

    add_executable (${LIBBTR_TARGET}-url ${URL})
    target_link_libraries (${LIBBTR_TARGET}-url ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-url ${LIBBTR_TARGET}-url)

    add_executable (${LIBBTR_TARGET}-btih ${BTIH})
    target_link_libraries (${LIBBTR_TARGET}-btih ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-btih ${LIBBTR_TARGET}-btih)

    add_executable (${LIBBTR_TARGET}-magnet ${MAGNET})
    target_link_libraries (${LIBBTR_TARGET}-magnet ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-magnet ${LIBBTR_TARGET}-magnet)
endif ()

if (LIBBTR_STATIC MATCHES true)
    add_executable (${LIBBTR_TARGET}-static-itoa ${ITOA})
    target_link_libraries (${LIBBTR_TARGET}-static-itoa ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-itoa ${LIBBTR_TARGET}-static-itoa)

    add_executable (${LIBBTR_TARGET}-static-url ${URL})
    target_link_libraries (${LIBBTR_TARGET}-static-url ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-url ${LIBBTR_TARGET}-static-url)

    add_executable (${LIBBTR_TARGET}-static-btih ${BTIH})
    target_link_libraries (${LIBBTR_TARGET}-static-btih ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-btih ${LIBBTR_TARGET}-static-btih)

    add_executable (${LIBBTR_TARGET}-static-magnet ${MAGNET})
    target_link_libraries (${LIBBTR_TARGET}-static-magnet ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-magnet ${LIBBTR_TARGET}-static-magnet)
endif ()