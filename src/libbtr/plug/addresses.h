// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#ifndef LIBBTR_ADDRESSES_H
#define LIBBTR_ADDRESSES_H

#include "../epoll.h"

#include "netinet/in.h"

#include <talloc2/utils/dynarr.h>
#include <talloc2/utils/buffer.h>

typedef struct bt_addresses_t {
    bt_epoll_event_type      type;
//     struct radix_tree_root * tree;
    talloc_buffer *          buffer;
    int *                    netlink_socket_fd;
} bt_addresses;

typedef struct bt_address_t {
    in_addr_t addr;
    char *    label;
} bt_address;

bt_addresses * bt_addresses_new          ( void * ctx );
uint8_t        bt_addresses_init         ( bt_addresses * addresses );
uint8_t        bt_addresses_add_to_epoll ( bt_addresses * addresses, int epoll_fd );
uint8_t        bt_addresses_read         ( bt_addresses * addresses );

#endif
