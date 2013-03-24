add_library(
	btbot SHARED
	magnet.c
)
target_link_libraries(btbot talloc)