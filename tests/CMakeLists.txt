add_executable(01-url-unescape 01-url-unescape.c)
target_link_libraries(01-url-unescape btbot talloc)

add_executable(02-btih 02-btih.c)
target_link_libraries(02-btih btbot talloc)

add_executable(03-magnet 03-magnet.c)
target_link_libraries(03-magnet btbot talloc)