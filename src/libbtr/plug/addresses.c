// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include "addresses.h"

#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <talloc2/ext/destructor.h>

static inline
uint8_t netlink_socket_close ( void * child_data, void * user_data )
{
    int * netlink_socket_fd = child_data;
    if ( close ( * netlink_socket_fd ) == -1 ) {
        return 1;
    }
    return 0;
}

bt_addresses * bt_addresses_new ( void * ctx )
{
    bt_addresses * addresses = talloc ( ctx, sizeof ( bt_addresses ) );
    if ( addresses == NULL ) {
        return NULL;
    }
    addresses->type = BT_EPOLL_EVENT_ADDRESSES;

    talloc_dynarr * arr = talloc_dynarr_new ( addresses, 1 );
    if ( arr == NULL ) {
        talloc_free ( addresses );
        return NULL;
    }
    addresses->arr = arr;

    int * socket_fd_ptr = talloc ( addresses, sizeof ( int ) );
    if ( socket_fd_ptr == NULL ) {
        talloc_free ( addresses );
        return NULL;
    }
    int socket_fd = socket ( AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE );
    if ( socket_fd == -1 ) {
        talloc_free ( addresses );
        return NULL;
    }
    * socket_fd_ptr = socket_fd;
    if ( talloc_add_destructor ( socket_fd_ptr, netlink_socket_close, NULL ) != 0 ) {
        close ( socket_fd );
        talloc_free ( addresses );
        return NULL;
    }
    addresses->netlink_socket_fd = socket_fd_ptr;

    talloc_buffer * buffer = talloc_buffer_new ( addresses );
    if ( buffer == NULL ) {
        talloc_free ( addresses );
        return NULL;
    }
    addresses->buffer = buffer;

    int socket_fd_flags = fcntl ( socket_fd, F_GETFL, 0 );
    if (
        socket_fd_flags == -1 ||
        fcntl ( socket_fd, F_SETFL, socket_fd_flags | O_NONBLOCK ) == -1
    ) {
        talloc_free ( addresses );
        return NULL;
    }

    struct sockaddr_nl local_address;
    local_address.nl_family = AF_NETLINK;
    local_address.nl_pad    = 0;
    local_address.nl_pid    = getpid();
    local_address.nl_groups = RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

    if ( bind ( socket_fd, ( struct sockaddr * ) &local_address, sizeof ( local_address ) ) == -1 ) {
        talloc_free ( addresses );
        return NULL;
    }

    return addresses;
}

uint8_t bt_addresses_init ( bt_addresses * addresses )
{
    struct sockaddr_nl kernel_address;
    kernel_address.nl_family = AF_NETLINK;
    kernel_address.nl_pad    = 0;
    kernel_address.nl_pid    = 0;
    kernel_address.nl_groups = 0;

    struct {
        struct nlmsghdr nlm;
        struct rtgenmsg rtgm;
    } request;
    request.nlm.nlmsg_len     = NLMSG_LENGTH ( sizeof ( struct rtgenmsg ) );
    request.nlm.nlmsg_type    = RTM_GETADDR;
    request.nlm.nlmsg_flags   = NLM_F_REQUEST | NLM_F_DUMP;
    request.nlm.nlmsg_seq     = 1;
    request.nlm.nlmsg_pid     = getpid();
    request.rtgm.rtgen_family = AF_INET;

    struct iovec iov;
    iov.iov_base = &request;
    iov.iov_len  = request.nlm.nlmsg_len;

    struct msghdr message;
    message.msg_flags      = 0;
    message.msg_controllen = 0;
    message.msg_iov        = &iov;
    message.msg_iovlen     = 1;
    message.msg_name       = &kernel_address;
    message.msg_namelen    = sizeof ( kernel_address );

    if ( sendmsg ( * addresses->netlink_socket_fd, ( struct msghdr * ) &message, 0 ) == -1 ) {
        return 1;
    }

    return 0;
}

uint8_t bt_addresses_add_to_epoll ( bt_addresses * addresses, int epoll_fd )
{
    struct epoll_event event;
    event.data.ptr = addresses;
    event.events   = EPOLLIN | EPOLLET;

    if ( epoll_ctl ( epoll_fd, EPOLL_CTL_ADD, * addresses->netlink_socket_fd, &event ) == -1 ) {
        return 1;
    }

    return 0;
}

static inline
bt_address * parse_address ( void * ctx, struct ifaddrmsg * message, size_t length )
{
    bt_address * address = talloc ( ctx, sizeof ( bt_address ) );
    if ( address == NULL ) {
        return NULL;
    }

    struct rtattr * attribute;
    size_t          payload;
    char *          label;

    bool found_label   = false;
    bool found_address = false;
    for ( attribute = IFLA_RTA ( message ) ; RTA_OK ( attribute, length ); attribute = RTA_NEXT ( attribute, length ) ) {
        switch ( attribute->rta_type ) {
        case IFA_ADDRESS:
        case IFA_LOCAL:
            memcpy ( &address->addr, RTA_DATA ( attribute ), RTA_PAYLOAD ( attribute ) );
            found_address = true;
            break;
        case IFA_LABEL:
            payload = RTA_PAYLOAD ( attribute );
            label = talloc ( address, sizeof ( char ) * payload );
            if ( label == NULL ) {
                talloc_free ( address );
                return NULL;
            }
            memcpy ( label, RTA_DATA ( attribute ), payload );
            address->label = label;
            found_label    = true;
            break;
        }
    }

    if ( found_label && found_address ) {
        return address;
    }
    talloc_free ( address );
    return NULL;
}

static inline
uint8_t process_data ( bt_addresses * addresses, struct nlmsghdr * data, bool add )
{
    struct ifaddrmsg * message = NLMSG_DATA ( data );
    uint32_t address_index     = message->ifa_index;
    if ( address_index == 0 ) {
        return 1;
    }
    address_index --;

    talloc_dynarr * arr = addresses->arr;

    if ( add ) {
        bt_address * address = parse_address ( addresses, message, IFLA_PAYLOAD ( data ) );
        if ( address == NULL ) {
            return 2;
        }

        size_t length = talloc_dynarr_get_length ( arr );

        // TODO address_index when aliases is active https://lkml.org/lkml/2013/7/14/19
        if ( talloc_dynarr_grow_and_set ( arr, address_index, address ) != 0 ) {
            talloc_free ( address );
            return 4;
        }
    } else {
        bt_address * address = talloc_dynarr_get ( arr, address_index );
        if ( talloc_free ( address ) != 0 ) {
            return 5;
        }

        if ( talloc_dynarr_delete ( arr, address_index ) != 0 ) {
            return 6;
        }
    }

    return 0;
}

static inline
uint8_t process ( bt_addresses * addresses )
{
    talloc_buffer * buffer = addresses->buffer;
    uint8_t * buf          = talloc_buffer_get ( buffer );
    size_t length          = talloc_buffer_get_length ( buffer );
    size_t current_length  = length;
    struct nlmsghdr * data;

    bool process_failed  = false;
    for ( data = ( struct nlmsghdr * ) buf; NLMSG_OK ( data, current_length ); data = NLMSG_NEXT ( data, current_length ) ) {
        switch ( data->nlmsg_type ) {
        case RTM_NEWADDR:
            if ( process_data ( addresses, data, true ) != 0 ) {
                process_failed = true;
            }
            break;
        case RTM_DELADDR:
            if ( process_data ( addresses, data, false ) != 0 ) {
                process_failed = true;
            }
            break;
        case NLMSG_DONE:
            if ( talloc_buffer_trim ( buffer ) != 0 ) {
                process_failed = true;
            }
            break;
        }
    }

    if ( talloc_buffer_readed ( buffer, length - current_length ) != 0 ) {
        return 1;
    }
    if ( process_failed ) {
        return 2;
    }

    return 0;
}

uint8_t bt_addresses_read ( bt_addresses * addresses )
{
    int socket_fd          = * addresses->netlink_socket_fd;
    talloc_buffer * buffer = addresses->buffer;

    uint8_t * buf;
    ssize_t next_data_length;

    while ( true ) {
        next_data_length = recv ( socket_fd, NULL, 0, MSG_PEEK | MSG_TRUNC );
        if ( next_data_length == -1 ) {
            if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
                break;
            }
            return 1;
        }  else if ( next_data_length == 0 ) {
            break;
        }
        buf = talloc_buffer_prepare ( buffer, next_data_length );
        if (
            buf == NULL ||
            read ( socket_fd, buf, next_data_length ) != next_data_length
        ) {
            return 2;
        }
        talloc_buffer_written ( buffer, next_data_length );
    }

    if ( process ( addresses ) != 0 ) {
        return 3;
    }

    return 0;
}
