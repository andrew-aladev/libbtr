set (DYNARR       01-dynarr.c)
set (LIST         02-list.c)
set (ITOA         03-itoa.c)
set (URL_UNESCAPE 04-url-unescape.c)
set (BTIH         05-btih.c)
set (MAGNET       06-magnet.c)

add_executable (01-dynarr ${DYNARR})
target_link_libraries (01-dynarr libbtr)
add_test (libbtr-dynarr 01-dynarr)

add_executable (02-list ${LIST})
target_link_libraries (02-list libbtr)
add_test (libbtr-list 02-list)

add_executable (03-itoa ${ITOA})
target_link_libraries (03-itoa libbtr)
add_test (libbtr-itoa 03-itoa)

add_executable (04-url-unescape ${URL_UNESCAPE})
target_link_libraries (04-url-unescape libbtr)
add_test (libbtr-url-unescape 04-url-unescape)

add_executable (05-btih ${BTIH})
target_link_libraries (05-btih libbtr)
add_test (libbtr-btih 05-btih)

add_executable (06-magnet ${MAGNET})
target_link_libraries (06-magnet libbtr)
add_test (libbtr-magnet 06-magnet)

if (LIBBTR_STATIC MATCHES true)
    add_executable (01-static-dynarr ${DYNARR})
    target_link_libraries (01-static-dynarr libbtr_static)
    add_test (libbtr-static-dynarr 01-static-dynarr)

    add_executable (02-static-list ${LIST})
    target_link_libraries (02-static-list libbtr_static)
    add_test (libbtr-static-list 02-static-list)

    add_executable (03-static-itoa ${ITOA})
    target_link_libraries (03-static-itoa libbtr_static)
    add_test (libbtr-static-itoa 03-static-itoa)

    add_executable (04-static-url-unescape ${URL_UNESCAPE})
    target_link_libraries (04-static-url-unescape libbtr_static)
    add_test (libbtr-static-url-unescape 04-static-url-unescape)

    add_executable (05-static-btih ${BTIH})
    target_link_libraries (05-static-btih libbtr_static)
    add_test (libbtr-static-btih 05-static-btih)

    add_executable (06-static-magnet ${MAGNET})
    target_link_libraries (06-static-magnet libbtr_static)
    add_test (libbtr-static-magnet 06-static-magnet)
endif ()