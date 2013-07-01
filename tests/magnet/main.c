// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <talloc2/tree.h>
#include <libbtr/magnet.h>

bool test_null ( void * ctx )
{
    if (
        bt_magnet_parse ( ctx, NULL ) != NULL          ||
        bt_magnet_parse ( ctx, "http:" ) != NULL       ||
        bt_magnet_parse ( ctx, "magnet:?" ) != NULL    ||
        bt_magnet_parse ( ctx, "magnet:?xt" ) != NULL  ||
        bt_magnet_parse ( ctx, "magnet:?xt=" ) != NULL ||
        bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:" ) != NULL ||
        bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:7018b750d7be55ba6b05" ) != NULL ||
        bt_magnet_parse ( ctx, "magnet:?xt=urn:tree:tiger:7N5OAMRNGMSSEUE3ORHOKWN4WWIQ5X4EBOOTLJY" ) != NULL ||
        bt_magnet_parse ( ctx, "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29" ) != NULL ||
        bt_magnet_parse ( ctx,
                          "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                          "&tr-1=udp%3A%2F%2Ftracker.openbittorrent.com%3A80" ) != NULL ||
        bt_magnet_parse ( ctx,
                          "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                          "&tr%=udp%3A%2F%2Ftracker.openbittorrent.com%3A80" ) != NULL ||
        bt_magnet_parse ( ctx,
                          "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                          "&tr.2=udp%3A%2F%2Ftracker.openbittorrent.com%3A80" ) != NULL ||
        bt_magnet_parse ( ctx,
                          "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                          "&tr.1=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                          "&tr=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                          "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                          "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80" ) != NULL ||
        bt_magnet_parse ( ctx,
                          "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                          "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                          "&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                          "&tr.1=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                          "&tr.2=udp%3A%2F%2Ftracker.istole.it%3A6969"
                          "&tr.3=udp%3A%2F%2Ftracker.ccc.de%3A80" ) != NULL ||
        bt_magnet_parse ( ctx,
                          "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                          "&tr.2=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                          "&tr.1=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                          "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                          "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80" ) != NULL
    ) {
        return false;
    }
    return true;
}

bool test_magnet_valid (
    void * ctx, char * str,
    uint8_t * hash, size_t hash_length,
    char * display_name,
    char ** trackers, size_t trackers_length,
    char ** webseeds, size_t webseeds_length )
{

    bt_magnet_info * info = bt_magnet_parse ( ctx, str );
    if ( info == NULL ) {
        return false;
    }
    if (
        info->hash == NULL ||
        info->hash->length != hash_length ||
        memcmp ( info->hash->binary, hash, hash_length ) != 0
    ) {
        talloc_free ( info );
        return false;
    }

    if ( display_name != NULL ) {
        if (
            info->display_name == NULL ||
            strcmp ( info->display_name, display_name ) != 0
        ) {
            talloc_free ( info );
            return false;
        }
    } else {
        if ( info->display_name != NULL ) {
            talloc_free ( info );
            return false;
        }
    }

    if (
        info->trackers == NULL ||
        talloc_list_get_length ( info->trackers ) != trackers_length
    ) {
        talloc_free ( info );
        return false;
    }
    talloc_list_item * item = info->trackers->last_item;
    size_t i = trackers_length - 1;
    while ( item != NULL ) {
        if ( strcmp ( trackers[i], item->data ) != 0 ) {
            talloc_free ( info );
            return false;
        }
        item = item->prev;
        i--;
    }

    if (
        info->webseeds == NULL ||
        talloc_list_get_length ( info->webseeds ) != webseeds_length
    ) {
        talloc_free ( info );
        return false;
    }
    item = info->webseeds->last_item;
    i = webseeds_length - 1;
    while ( item != NULL ) {
        if ( strcmp ( webseeds[i], item->data ) != 0 ) {
            talloc_free ( info );
            return false;
        }
        item = item->prev;
        i--;
    }

    talloc_free ( info );
    return true;
}

bool test_valid ( void * ctx )
{
    uint8_t hash_1[] = { 0x42, 0x42, 0x37, 0x8E, 0x41, 0x9F, 0x21, 0x95, 0x85, 0x09, 0xF3, 0x6F, 0xF5, 0xA2, 0x72, 0xC9, 0xF3, 0x0F, 0x97, 0xA8 };
    uint8_t hash_2[] = { 0x7F, 0x9D, 0x36, 0x7F, 0x5D, 0x5D, 0x7F, 0x57, 0x36, 0xF5, 0xBE, 0x5C, 0x6B, 0xC7, 0xB9, 0x73, 0x67, 0xDA, 0xE7, 0x46, 0x9B, 0x71, 0xB7, 0xF9, 0xF5, 0xBD, 0x5D, 0xDB, 0xBE, 0x1F};
    uint8_t hash_3[] = { 0xD9, 0xFF, 0x37, 0xDD, 0xCE, 0xDA, 0xD1, 0xAE, 0x75, 0xF3, 0xDD, 0x36, 0xDF, 0xC7, 0xBA, 0xF1, 0xCE, 0xF4, 0xE7, 0xBE, 0xBC, 0xE9, 0xEE, 0x3A, 0x7B, 0xD6, 0xF5, 0xE5, 0xFD, 0x7B };
    char * trackers[] = {"udp://tracker.openbittorrent.com:80", "udp://tracker.publicbt.com:80", "udp://tracker.istole.it:6969", "udp://tracker.ccc.de:80"};
    char * webseeds[] = {"http://dl.com/path/to/file", "http://dl.org/path/to/file"};
    if (
        !test_magnet_valid ( ctx,
                             "magnet:?xt=urn:btih:IJBDPDSBT4QZLBIJ6NX7LITSZHZQ7F5I",
                             hash_1, sizeof ( hash_1 ),
                             NULL,
                             NULL, 0, NULL, 0 ) ||
        !test_magnet_valid ( ctx,
                             "magnet:?xt=urn:btih:f502f11df1c29b5ca8e5c2fa50abcbf59b1d274f"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr=udp%3A%2F%2Ftracker.ccc.de%3A80",
                             hash_2, sizeof ( hash_2 ),
                             "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)",
                             trackers, 4, NULL, 0 ) ||
        !test_magnet_valid ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.1=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80",
                             hash_3, sizeof ( hash_3 ),
                             "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)",
                             trackers, 4, NULL, 0 ) ||
        !test_magnet_valid ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.0=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.1=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.3=udp%3A%2F%2Ftracker.ccc.de%3A80",
                             hash_3, sizeof ( hash_3 ),
                             "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)",
                             trackers, 4, NULL, 0 ) ||
        !test_magnet_valid ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.1=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80"
                             "&ws=http%3A%2F%2Fdl.com%2Fpath%2Fto%2Ffile"
                             "&ws=http%3A%2F%2Fdl.org%2Fpath%2Fto%2Ffile",
                             hash_3, sizeof ( hash_3 ),
                             "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)",
                             trackers, 4, webseeds, 2 )
    ) {
        return false;
    }
    return true;
}

int main()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        talloc_free ( ctx );
        return 1;
    }

    if ( !test_null ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }
    if ( !test_valid ( ctx ) ) {
        talloc_free ( ctx );
        return 3;
    }

    talloc_free ( ctx );
    return 0;
}
