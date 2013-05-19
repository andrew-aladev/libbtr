add_executable (01-dynarr 01-dynarr.c)
target_link_libraries (01-dynarr libbtr talloc)
add_test (libbtr-dynarr 01-dynarr)

add_executable (02-list 02-list.c)
target_link_libraries (02-list libbtr talloc)
add_test (libbtr-list 02-list)

add_executable (03-itoa 03-itoa.c)
target_link_libraries (03-itoa libbtr talloc)
add_test (libbtr-itoa 03-itoa)

add_executable (04-url-unescape 04-url-unescape.c)
target_link_libraries (04-url-unescape libbtr talloc)
add_test (libbtr-url-unescape 04-url-unescape)

add_executable (05-btih 05-btih.c)
target_link_libraries (05-btih libbtr talloc)
add_test (libbtr-btih 05-btih)

add_executable (06-magnet 06-magnet.c)
target_link_libraries (06-magnet libbtr talloc)
add_test (libbtr-magnet 06-magnet)