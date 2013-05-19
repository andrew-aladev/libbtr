// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <talloc/helpers.h>
#include <libbtr/utils/url.h>

bool test_null ( void * ctx ) {
    if (
        ! (
            bt_unescape ( ctx, NULL ) == NULL &&
            bt_unescape ( ctx, "%" )  == NULL &&
            bt_unescape ( ctx, "%1" ) == NULL
        )
    ) {
        return false;
    }
    return true;
}

bool test_url ( void * ctx, char * encoded_url, char * decoded_url ) {
    char * url = bt_unescape ( ctx, encoded_url );
    if (
        ! (
            url != NULL &&
            !strcmp ( url, decoded_url )
        )
    ) {
        return false;
    }
    talloc_free ( url );
    return true;
}

bool test_urls ( void * ctx ) {
    if (
        ! (
            test_url ( ctx,
                       "udp%3A%2F%2Ftracker.openbittorrent.com%3A80",
                       "udp://tracker.openbittorrent.com:80" ) &&
            test_url ( ctx,
                       "music%3A%2F%2Ftsoi.com%2Fsadness%3A8183",
                       "music://tsoi.com/sadness:8183" )
        )
    ) {
        return false;
    }
    return true;
}

bool test_components ( void * ctx ) {
    if (
        ! (
            test_url ( ctx,
                       "Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29",
                       "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) &&
            test_url ( ctx,
                       "Butusov%20%3A%20breath",
                       "Butusov : breath" )
        )
    ) {
        return false;
    }
    return true;
}

int main() {
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        talloc_free ( ctx );
        return 1;
    }

    if ( !test_null ( ctx ) ) {
        talloc_free ( ctx );
        return 1;
    }
    if ( !test_urls ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }
    if ( !test_components ( ctx ) ) {
        talloc_free ( ctx );
        return 3;
    }

    talloc_free ( ctx );
    return 0;
}


