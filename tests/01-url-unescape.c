// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <talloc/tree.h>
#include <libbtr/utils/url.h>

int main() {
    void * ctx = talloc_new ( NULL );
    if ( !ctx ) {
        talloc_free ( ctx );
        return 1;
    }
    char * url;

    // should return NULL
    url = bt_unescape ( ctx, NULL );
    if ( url ) {
        talloc_free ( ctx );
        return 1;
    }
    url = bt_unescape ( ctx, "%" );
    if ( url ) {
        talloc_free ( ctx );
        return 1;
    }
    url = bt_unescape ( ctx, "%1" );
    if ( url ) {
        talloc_free ( ctx );
        return 2;
    }

    //should return unescaped url
    url = bt_unescape ( ctx, "Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29" );
    if ( !url || strcmp ( url, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ) {
        talloc_free ( ctx );
        return 3;
    }
    talloc_free ( url );

    url = bt_unescape ( ctx, "udp%3A%2F%2Ftracker.openbittorrent.com%3A80" );
    if ( !url || strcmp ( url, "udp://tracker.openbittorrent.com:80" ) ) {
        talloc_free ( ctx );
        return 4;
    }
    talloc_free ( url );

    talloc_free ( ctx );
    return 0;
}


