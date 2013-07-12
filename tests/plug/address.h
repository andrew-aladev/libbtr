// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>

#include <sys/epoll.h>

#include <libbtr/addresses.h>

bool test_address ( void * ctx )
{
    bt_addresses * addresses = bt_addresses_new ( ctx );
    if ( addresses == NULL ) {
        return false;
    }
    if ( bt_addresses_init_list ( addresses ) != 0 ) {
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

//     struct epoll_event event;
//     int ready_events_count;
//     while ( true ) {
//         ready_events_count = epoll_wait ( epoll_fd, &event, 1, -1 );
//         if ( ready_events_count != 1 ) {
//             talloc_free ( addresses );
//             return false;
//         }
//         bt_epoll_event_type * type = event.data.ptr;
//         if ( * type != BT_EPOLL_EVENT_ADDRESSES ) {
//             talloc_free ( addresses );
//             return false;
//         }
//         if ( bt_addresses_read ( addresses ) != 0 ) {
//             talloc_free ( addresses );
//             return false;
//         }
//     }

    return true;
}

// #define BUFFER_SIZE 512
//
// typedef struct request_t {
//     struct nlmsghdr nlm;
//     struct rtgenmsg rtgm;
// } request;
//
// bool test_address ( void * ctx )
// {
//     int socket_fd = socket ( AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE );
//     if ( socket_fd == -1 ) {
//         return false;
//     }
//
//     int yes = 1;
//     int socket_fd_flags = fcntl ( socket_fd, F_GETFL, 0 );
//     if (
//         socket_fd_flags == -1 ||
//         fcntl ( socket_fd, F_SETFL, socket_fd_flags | O_NONBLOCK ) == -1 ||
//         setsockopt ( socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof ( yes ) ) == -1
//     ) {
//         return false;
//     }
//
//     struct sockaddr_nl local_address;
//     local_address.nl_family = AF_NETLINK;
//     local_address.nl_pad    = 0;
//     local_address.nl_pid    = getpid();
//     local_address.nl_groups = 0;
//
//     if ( bind ( socket_fd, ( struct sockaddr * ) &local_address, sizeof ( local_address ) ) == -1 ) {
//         return false;
//     }
//
//     struct sockaddr_nl kernel_address;
//     kernel_address.nl_family = AF_NETLINK;
//     kernel_address.nl_pad    = 0;
//     kernel_address.nl_pid    = 0;
//     kernel_address.nl_groups = 0;
//
//     request req;
//     req.nlm.nlmsg_len     = NLMSG_LENGTH ( sizeof ( struct rtgenmsg ) );
//     req.nlm.nlmsg_type    = RTM_GETADDR;
//     req.nlm.nlmsg_flags   = NLM_F_REQUEST | NLM_F_DUMP;
//     req.nlm.nlmsg_seq     = 1;
//     req.nlm.nlmsg_pid     = getpid();
//     req.rtgm.rtgen_family = AF_INET;
//
//     struct iovec iov;
//     iov.iov_base = &req;
//     iov.iov_len  = req.nlm.nlmsg_len;
//
//     struct msghdr message;
//     message.msg_flags      = 0;
//     message.msg_controllen = 0;
//     message.msg_iov        = &iov;
//     message.msg_iovlen     = 1;
//     message.msg_name       = &kernel_address;
//     message.msg_namelen    = sizeof ( kernel_address );
//
//     if ( sendmsg ( socket_fd, ( struct msghdr * ) &message, 0 ) == -1 ) {
//         return false;
//     }
//
//     int epoll_fd = epoll_create ( 1 );
//     if ( epoll_fd == -1 ) {
//         return 1;
//     }
//
//     struct epoll_event event;
//     event.data.fd = socket_fd;
//     event.events  = EPOLLIN | EPOLLET;
//
//     if ( epoll_ctl ( epoll_fd, EPOLL_CTL_ADD, socket_fd, &event ) == -1 ) {
//         return false;
//     }
//
//     struct epoll_event events[5];
//     int ready_events_count, index;
//     char buffer[200];
//     ssize_t count;
//     while ( true ) {
//         ready_events_count = epoll_wait ( epoll_fd, events, 5, -1 );
//         if ( ready_events_count == -1 ) {
//             return false;
//         }
//         for ( index = 0; index < ready_events_count; index ++ ) {
//             event = events[index];
//             if ( ( event.events & EPOLLIN ) != 0 ) {
//                 int fd = event.data.fd;
//                 while ( true ) {
//                     printf ( "%s\n", "start reading" );
//                     printf ( "expecting %zd\n", recv ( fd, NULL, 0, MSG_PEEK | MSG_TRUNC ) );
//                     count = read ( fd, buffer, 200 );
//                     if ( count == -1 ) {
//                         if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
//                             break;
//                         }
//                         return false;
//                     } else if ( count == 0 ) {
//                         break;
//                     }
//                     printf ( "readed %zd\n", count );
//                 }
//             }
//         }
//     }
//
//     return true;
// }
