add_executable(01-dynarr 01-dynarr.c)
target_link_libraries(01-dynarr libbtr talloc)
add_test(btr-dynarr 01-dynarr)

add_executable(02-itoa 02-itoa.c)
target_link_libraries(02-itoa libbtr talloc)
add_test(btr-itoa 02-itoa)

add_executable(03-url-unescape 03-url-unescape.c)
target_link_libraries(03-url-unescape libbtr talloc)
add_test(btr-url-unescape 03-url-unescape)

add_executable(04-btih 04-btih.c)
target_link_libraries(04-btih libbtr talloc)
add_test(btr-btih 04-btih)

add_executable(05-magnet 05-magnet.c)
target_link_libraries(05-magnet libbtr talloc)
add_test(btr-magnet 05-magnet)