#ifndef MAGNET_H
#define MAGNET_H

#include <talloc.h>

// specification http://www.bittorrent.org/beps/bep_0009.html
#define HASH_LENGTH 40

typedef struct bt_magnet_info {
    char    hash[HASH_LENGTH + 1];
    char *  display_name;
    char ** trackers;
    size_t  trackers_count;
}
bt_magnet_info;

bt_magnet_info * bt_magnet_parse ( TALLOC_CTX * root, char * uri );

#endif
