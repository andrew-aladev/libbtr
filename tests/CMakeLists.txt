set (LIST         02-list.c)
set (ITOA         03-itoa.c)
set (URL_UNESCAPE 04-url-unescape.c)
set (BTIH         05-btih.c)
set (MAGNET       06-magnet.c)

if (LIBBTR_SHARED MATCHES true)
    add_executable (${LIBBTR_TARGET}-list ${LIST})
    target_link_libraries (${LIBBTR_TARGET}-list ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-list ${LIBBTR_TARGET}-list)

    add_executable (${LIBBTR_TARGET}-itoa ${ITOA})
    target_link_libraries (${LIBBTR_TARGET}-itoa ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-itoa ${LIBBTR_TARGET}-itoa)

    add_executable (${LIBBTR_TARGET}-url-unescape ${URL_UNESCAPE})
    target_link_libraries (${LIBBTR_TARGET}-url-unescape ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-url-unescape ${LIBBTR_TARGET}-url-unescape)

    add_executable (${LIBBTR_TARGET}-btih ${BTIH})
    target_link_libraries (${LIBBTR_TARGET}-btih ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-btih ${LIBBTR_TARGET}-btih)

    add_executable (${LIBBTR_TARGET}-magnet ${MAGNET})
    target_link_libraries (${LIBBTR_TARGET}-magnet ${LIBBTR_TARGET})
    add_test (${LIBBTR_TARGET}-magnet ${LIBBTR_TARGET}-magnet)
endif ()

if (LIBBTR_STATIC MATCHES true)
    add_executable (${LIBBTR_TARGET}-static-list ${LIST})
    target_link_libraries (${LIBBTR_TARGET}-static-list ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-list ${LIBBTR_TARGET}-static-list)

    add_executable (${LIBBTR_TARGET}-static-itoa ${ITOA})
    target_link_libraries (${LIBBTR_TARGET}-static-itoa ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-itoa ${LIBBTR_TARGET}-static-itoa)

    add_executable (${LIBBTR_TARGET}-static-url-unescape ${URL_UNESCAPE})
    target_link_libraries (${LIBBTR_TARGET}-static-url-unescape ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-url-unescape ${LIBBTR_TARGET}-static-url-unescape)

    add_executable (${LIBBTR_TARGET}-static-btih ${BTIH})
    target_link_libraries (${LIBBTR_TARGET}-static-btih ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-btih ${LIBBTR_TARGET}-static-btih)

    add_executable (${LIBBTR_TARGET}-static-magnet ${MAGNET})
    target_link_libraries (${LIBBTR_TARGET}-static-magnet ${LIBBTR_TARGET}_static)
    add_test (${LIBBTR_TARGET}-static-magnet ${LIBBTR_TARGET}-static-magnet)
endif ()