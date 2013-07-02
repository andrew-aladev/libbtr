// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#ifndef LIBBTR_MAGNET_H
#define LIBBTR_MAGNET_H

#include <stdint.h>
#include <talloc2/utils/list.h>

#include "btih.h"

// See specification http://www.bittorrent.org/beps/bep_0009.html
//   additions https://trac.transmissionbt.com/ticket/5134
//   additions https://trac.transmissionbt.com/ticket/2631

#define BT_HASH_BASE32_SRC  32
#define BT_HASH_BASE32_INFO 20

#define BT_HASH_BASE64_SRC  40
#define BT_HASH_BASE64_INFO 30

typedef struct bt_magnet_info_t {
    bt_hash      * hash;
    char         * display_name;
    talloc_list  * trackers;
    talloc_list  * webseeds;
} bt_magnet_info;

bt_magnet_info * bt_magnet_parse ( void * ctx, char * uri );

#endif
