// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#ifndef LIBBTR_MAGNET_KEYLIST_H
#define LIBBTR_MAGNET_KEYLIST_H

#include "../utils/list.h"
#include "../utils/itoa.h"

#include <stdint.h>
#include <string.h>

/*

STRICT
"" => value_1
"" => value_2
"" => value_3

or

ZERO_INDEXED
".0" => value_1
".1" => value_2
".2" => value_3

or

ONE_INDEXED
".1" => value_1
".2" => value_2
".3" => value_3

*/

enum {
    BT_MAGNET_KEYLIST_STRICT = 1,
    BT_MAGNET_KEYLIST_ZERO_INDEXED,
    BT_MAGNET_KEYLIST_ONE_INDEXED
};

typedef struct bt_magnet_keylist_t {
    bt_list * list;
    uint8_t mode;
} bt_magnet_keylist;

inline
bt_magnet_keylist * bt_magnet_keylist_new ( bt_list * list ) {
    bt_magnet_keylist * keylist = talloc ( list, sizeof ( bt_magnet_keylist ) );
    if ( keylist == NULL ) {
        return NULL;
    }
    keylist->list = list;
    keylist->mode = 0;
    return keylist;
}

inline
uint8_t bt_magnet_keylist_indexed ( bt_magnet_keylist * keylist, char * key, size_t key_size ) {
    if ( * key != '.' ) {
        return 1;
    }
    key++;
    key_size--;

    if ( keylist->mode ) {
        if ( keylist->mode == BT_MAGNET_KEYLIST_STRICT ) {
            return 2;
        }
        size_t index = bt_list_get_length ( keylist->list );
        if ( keylist->mode == BT_MAGNET_KEYLIST_ONE_INDEXED ) {
            index++;
        }
        char * index_str = bt_size_t_to_str ( keylist, index );
        if ( strlen ( index_str ) != key_size ) {
            talloc_free ( index_str );
            return 3;
        }
        if ( strncmp ( key, index_str, key_size ) ) {
            talloc_free ( index_str );
            return 4;
        }
        talloc_free ( index_str );
    } else {
        if ( key_size == 1 ) {
            if ( * key == '0' ) {
                keylist->mode = BT_MAGNET_KEYLIST_ZERO_INDEXED;
            } else if ( * key == '1' ) {
                keylist->mode = BT_MAGNET_KEYLIST_ONE_INDEXED;
            } else {
                return 5;
            }
        } else {
            return 6;
        }
    }
    return 0;
}

inline
uint8_t bt_magnet_keylist_strict ( bt_magnet_keylist * keylist ) {
    if ( keylist->mode ) {
        if ( keylist->mode != BT_MAGNET_KEYLIST_STRICT ) {
            return 1;
        }
    } else {
        keylist->mode = BT_MAGNET_KEYLIST_STRICT;
    }
    return 0;
}

inline
uint8_t bt_magnet_keylist_append ( bt_magnet_keylist * keylist, char * key, size_t key_size, char * value ) {
    if ( key_size ) {
        if ( bt_magnet_keylist_indexed ( keylist, key, key_size ) ) {
            return 1;
        }
    } else {
        if ( bt_magnet_keylist_strict ( keylist ) ) {
            return 2;
        }
    }

    if ( bt_list_append ( keylist->list, value ) ) {
        return 3;
    }
    return 0;
}

#endif
