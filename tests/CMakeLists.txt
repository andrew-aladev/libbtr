add_executable(01-url-unescape 01-url-unescape.c)
target_link_libraries(01-url-unescape btlib talloc)

add_executable(02-btih 02-btih.c)
target_link_libraries(02-btih btlib talloc)

add_executable(03-itoa 03-itoa.c)
target_link_libraries(03-itoa btlib talloc)

add_executable(04-magnet 04-magnet.c)
target_link_libraries(04-magnet btlib talloc)