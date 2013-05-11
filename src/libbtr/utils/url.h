// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#ifndef UTILS_URL_H
#define UTILS_URL_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <talloc/tree.h>
#include <talloc/helpers.h>

// see http://www.ietf.org/rfc/rfc1738.txt
extern inline
char * bt_unescape ( void * ctx, char * url ) {
    if ( url == NULL ) {
        return NULL;
    }

    size_t url_length = strlen ( url );
    char * buffer     = talloc ( ctx, sizeof ( char ) * ( url_length + 1 ) );
    if ( buffer == NULL ) {
        return NULL;
    }

    char * walk_url    = url;
    char * walk_buffer = buffer;
    char * end_url     = url + url_length;
    char ch;

    while ( walk_url != end_url ) {
        ch = *walk_url;
        if ( ch == '+' ) {
            ch = ' ';
        } else if ( ch == '%' ) {
            walk_url++;
            if ( end_url - walk_url < 2 ) {
                // bad hex
                talloc_free ( buffer );
                return NULL;
            }

            char * hex = talloc_strndup ( buffer, walk_url, 2 );
            if ( hex == NULL ) {
                talloc_free ( buffer );
                return NULL;
            }
            ch = ( char ) strtol ( hex, NULL, 16 );
            if ( !ch ) {
                talloc_free ( buffer );
                return NULL;
            }
            talloc_free ( hex );
            walk_url++;
        }

        *walk_buffer = ch;
        walk_url++;
        walk_buffer++;
    }

    *walk_buffer = '\0';
    char * result = talloc_strdup ( ctx, buffer );
    if ( result == NULL ) {
        talloc_free ( buffer );
        return NULL;
    }

    talloc_free ( buffer );
    return result;
}

#endif

