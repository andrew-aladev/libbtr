add_executable(01-url-unescape 01-url-unescape.c)
target_link_libraries(01-url-unescape libbtr talloc)
add_test(url-unescape 01-url-unescape)

add_executable(02-btih 02-btih.c)
target_link_libraries(02-btih libbtr talloc)
add_test(btih 02-btih)

add_executable(03-itoa 03-itoa.c)
target_link_libraries(03-itoa libbtr talloc)
add_test(itoa 03-itoa)

add_executable(04-magnet 04-magnet.c)
target_link_libraries(04-magnet libbtr talloc)
add_test(magnet 04-magnet)