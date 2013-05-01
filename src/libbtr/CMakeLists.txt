add_library(
    libbtr SHARED
    magnet.c
    btih.c
)
target_link_libraries(libbtr talloc m)