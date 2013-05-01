add_library(
    btlib SHARED
    magnet.c
    btih.c
)
target_link_libraries(btlib talloc m)