// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <talloc.h>
#include <btbot/magnet.h>

int main() {
    TALLOC_CTX * ctx = talloc_new ( NULL );
    if ( !ctx ) {
        talloc_free ( ctx );
        return 1;
    }
    bt_magnet_info * info;

    // should return NULL
    info = bt_magnet_parse ( ctx, NULL );
    if ( info ) {
        talloc_free ( ctx );
        return 2;
    }
    info = bt_magnet_parse ( ctx, "http:" );
    if ( info ) {
        talloc_free ( ctx );
        return 3;
    }
    info = bt_magnet_parse ( ctx, "magnet:?" );
    if ( info ) {
        talloc_free ( ctx );
        return 4;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt" );
    if ( info ) {
        talloc_free ( ctx );
        return 4;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=" );
    if ( info ) {
        talloc_free ( ctx );
        return 5;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:" );
    if ( info ) {
        talloc_free ( ctx );
        return 6;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:7018b750d7be55ba6b05" );
    if ( info ) {
        talloc_free ( ctx );
        return 7;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:tree:tiger:7N5OAMRNGMSSEUE3ORHOKWN4WWIQ5X4EBOOTLJY" );
    if ( info ) {
        talloc_free ( ctx );
        return 8;
    }
    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.1=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    if ( info ) {
        talloc_free ( ctx );
        return 9;
    }
    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.1=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.3=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    if ( info ) {
        talloc_free ( ctx );
        return 10;
    }
    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.2=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.1=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    if ( info ) {
        talloc_free ( ctx );
        return 11;
    }

    // should return valid info
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:IJBDPDSBT4QZLBIJ6NX7LITSZHZQ7F5I" );
    uint8_t hash_1[] = { 0x42, 0x42, 0x37, 0x8E, 0x41, 0x9F, 0x21, 0x95, 0x85, 0x09, 0xF3, 0x6F, 0xF5, 0xA2, 0x72, 0xC9, 0xF3, 0x0F, 0x97, 0xA8 };
    if (
        !info ||
        info->hash_length != sizeof ( hash_1 ) ||
        memcmp ( info->hash, hash_1, sizeof ( hash_1 ) )
    ) {
        talloc_free ( ctx );
        return 12;
    }
    talloc_free ( info );

    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:f502f11df1c29b5ca8e5c2fa50abcbf59b1d274f"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    uint8_t hash_2[] = { 0x7F, 0x9D, 0x36, 0x7F, 0x5D, 0x5D, 0x7F, 0x57, 0x36, 0xF5, 0xBE, 0x5C, 0x6B, 0xC7, 0xB9, 0x73, 0x67, 0xDA, 0xE7, 0x46, 0x9B, 0x71, 0xB7, 0xF9, 0xF5, 0xBD, 0x5D, 0xDB, 0xBE, 0x1F};
    if (
        !info ||
        info->hash_length != sizeof ( hash_2 ) ||
        memcmp ( info->hash, hash_2, sizeof ( hash_2 ) ) ||
        info->trackers_count != 4 ||
        strcmp ( info->display_name, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ||
        strcmp ( info->trackers[0],  "udp://tracker.openbittorrent.com:80" ) ||
        strcmp ( info->trackers[1],  "udp://tracker.publicbt.com:80" ) ||
        strcmp ( info->trackers[2],  "udp://tracker.istole.it:6969" ) ||
        strcmp ( info->trackers[3],  "udp://tracker.ccc.de:80" )
    ) {
        talloc_free ( ctx );
        return 13;
    }
    talloc_free ( info );

    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.1=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    uint8_t hash_3[] = { 0xD9, 0xFF, 0x37, 0xDD, 0xCE, 0xDA, 0xD1, 0xAE, 0x75, 0xF3, 0xDD, 0x36, 0xDF, 0xC7, 0xBA, 0xF1, 0xCE, 0xF4, 0xE7, 0xBE, 0xBC, 0xE9, 0xEE, 0x3A, 0x7B, 0xD6, 0xF5, 0xE5, 0xFD, 0x7B };
    if (
        !info ||
        info->hash_length != sizeof ( hash_3 ) ||
        memcmp ( info->hash, hash_3, sizeof ( hash_3 ) ) ||
        info->trackers_count != 4 ||
        strcmp ( info->display_name, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ||
        strcmp ( info->trackers[0],  "udp://tracker.openbittorrent.com:80" ) ||
        strcmp ( info->trackers[1],  "udp://tracker.publicbt.com:80" ) ||
        strcmp ( info->trackers[2],  "udp://tracker.istole.it:6969" ) ||
        strcmp ( info->trackers[3],  "udp://tracker.ccc.de:80" )
    ) {
        talloc_free ( ctx );
        return 14;
    }
    talloc_free ( info );

    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.0=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.1=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.3=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    if (
        !info ||
        info->hash_length != sizeof ( hash_3 ) ||
        memcmp ( info->hash, hash_3, sizeof ( hash_3 ) ) ||
        info->trackers_count != 4 ||
        strcmp ( info->display_name, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ||
        strcmp ( info->trackers[0],  "udp://tracker.openbittorrent.com:80" ) ||
        strcmp ( info->trackers[1],  "udp://tracker.publicbt.com:80" ) ||
        strcmp ( info->trackers[2],  "udp://tracker.istole.it:6969" ) ||
        strcmp ( info->trackers[3],  "udp://tracker.ccc.de:80" )
    ) {
        talloc_free ( ctx );
        return 15;
    }
    talloc_free ( info );

    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:2f833c7a0a51890238e68c7057686e46e9b15f17"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.0=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.1=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.3=udp%3A%2F%2Ftracker.ccc.de%3A80"
                             "&ws=http://dl.com/path/to/file"
                             "&ws=http://dl.org/path/to/file" );
    if (
        !info ||
        info->hash_length != sizeof ( hash_3 ) ||
        memcmp ( info->hash, hash_3, sizeof ( hash_3 ) ) ||
        info->trackers_count != 4 ||
        strcmp ( info->display_name, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ||
        strcmp ( info->trackers[0],  "udp://tracker.openbittorrent.com:80" ) ||
        strcmp ( info->trackers[1],  "udp://tracker.publicbt.com:80" ) ||
        strcmp ( info->trackers[2],  "udp://tracker.istole.it:6969" ) ||
        strcmp ( info->trackers[3],  "udp://tracker.ccc.de:80" ) ||
        info->webseeds_count != 2 ||
        strcmp ( info->webseeds[0],  "http://dl.com/path/to/file" ) ||
        strcmp ( info->webseeds[1],  "http://dl.org/path/to/file" )
    ) {
        talloc_free ( ctx );
        return 16;
    }
    talloc_free ( info );

    talloc_free ( ctx );
    return 0;
}


