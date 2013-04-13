add_library(
	btbot SHARED
	magnet.c
	btih.c
)
target_link_libraries(btbot talloc m)