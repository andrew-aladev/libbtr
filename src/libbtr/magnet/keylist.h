// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#ifndef LIBBTR_MAGNET_KEYLIST_H
#define LIBBTR_MAGNET_KEYLIST_H

#include "../utils/list.h"

#include <stdint.h>

enum {
    BT_MAGNET_KEYLIST_EXPLICIT = 1,
    BT_MAGNET_KEYLIST_IMPLICIT,
    BT_MAGNET_KEYLIST_MIXED
};

typedef struct bt_magnet_keylist_t {
    uint8_t mode;
    bt_list * list;
} bt_magnet_keylist;

bt_magnet_keylist * bt_magnet_keylist_new ( void * ctx ) {
    return NULL;
}

#endif
