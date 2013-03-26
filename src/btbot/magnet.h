// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

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
