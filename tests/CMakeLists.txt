set (LIST         02-list.c)
set (ITOA         03-itoa.c)
set (URL_UNESCAPE 04-url-unescape.c)
set (BTIH         05-btih.c)
set (MAGNET       06-magnet.c)

if (LIBBTR_SHARED MATCHES true)
    add_executable (02-list ${LIST})
    target_link_libraries (02-list ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-list 02-list)

    add_executable (03-itoa ${ITOA})
    target_link_libraries (03-itoa ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-itoa 03-itoa)

    add_executable (04-url-unescape ${URL_UNESCAPE})
    target_link_libraries (04-url-unescape ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-url-unescape 04-url-unescape)

    add_executable (05-btih ${BTIH})
    target_link_libraries (05-btih ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-btih 05-btih)

    add_executable (06-magnet ${MAGNET})
    target_link_libraries (06-magnet ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-magnet 06-magnet)
endif ()

if (LIBBTR_STATIC MATCHES true)
    add_executable (02-static-list ${LIST})
    target_link_libraries (02-static-list ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-list 02-static-list)

    add_executable (03-static-itoa ${ITOA})
    target_link_libraries (03-static-itoa ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-itoa 03-static-itoa)

    add_executable (04-static-url-unescape ${URL_UNESCAPE})
    target_link_libraries (04-static-url-unescape ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-url-unescape 04-static-url-unescape)

    add_executable (05-static-btih ${BTIH})
    target_link_libraries (05-static-btih ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-btih 05-static-btih)

    add_executable (06-static-magnet ${MAGNET})
    target_link_libraries (06-static-magnet ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-magnet 06-static-magnet)
endif ()