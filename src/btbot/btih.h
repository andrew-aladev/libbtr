// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

#ifndef BTIH_H
#define BTIH_H

#include <math.h>
#include <string.h>
#include <talloc.h>

extern inline
char * bt_base32_to_hex ( TALLOC_CTX * ctx, char * encoded ) {
    size_t encoded_length = strlen ( encoded );
    size_t result_length  = ( size_t ) ceil ( 0.625 * encoded_length );
    char * result = talloc_array ( ctx, char, result_length );
    if ( !result ) {
        return NULL;
    }

    char ch;
    char * walk     = encoded;
    char * walk_end = walk + encoded_length;

    char number_offset = '2' - 26;
    while ( walk != walk_end ) {
        ch = *walk;

        if ( ch >= 'a' && ch <= 'z' ) {
            ch = ( ch & 0x1F ) - 1;
        } else if ( ch >= '2' && ch <= '7' ) {
            ch -= number_offset;
        } else {
            talloc_free ( result );
            return NULL;
        }

        walk++;
    }

    return NULL;
}

#endif
