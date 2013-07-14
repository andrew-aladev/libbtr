// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>

#include <sys/epoll.h>

#include <libbtr/plug/addresses.h>

bool test_address ( void * ctx )
{
    bt_addresses * addresses = bt_addresses_new ( ctx );
    if ( addresses == NULL ) {
        return false;
    }
    if ( bt_addresses_init ( addresses ) != 0 ) {
        talloc_free ( addresses );
        return false;
    }
    int epoll_fd = epoll_create ( 1 );
    if ( epoll_fd == -1 ) {
        talloc_free ( addresses );
        return false;
    }
    if ( bt_addresses_add_to_epoll ( addresses, epoll_fd ) != 0 ) {
        talloc_free ( addresses );
        return false;
    }

    struct epoll_event event;
    int ready_events_count;
    while ( true ) {
        ready_events_count = epoll_wait ( epoll_fd, &event, 1, -1 );
        if ( ready_events_count != 1 ) {
            talloc_free ( addresses );
            return false;
        }
        bt_epoll_event_type * type = event.data.ptr;
        if ( * type != BT_EPOLL_EVENT_ADDRESSES ) {
            talloc_free ( addresses );
            return false;
        }
        if ( bt_addresses_read ( addresses ) != 0 ) {
            talloc_free ( addresses );
            return false;
        }

        // test only initial interfaces list
        break;
    }

    return true;
}
