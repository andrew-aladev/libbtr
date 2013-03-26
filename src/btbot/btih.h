// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

#ifndef BTIH_H
#define BTIH_H

#include <math.h>
#include <string.h>
#include <stdint.h>
#include <talloc.h>

static const int BASE32_SMALL_ALPHA_OFFSET = - 'a';
static const int BASE32_BIG_ALPHA_OFFSET   = - 'A';
static const int BASE32_NUMBER_OFFSET      = 26 - '2';

static inline
int8_t from_base32 ( int8_t ch ) {
    if ( ch >= 'a' && ch <= 'z' ) {
        return ch + BASE32_SMALL_ALPHA_OFFSET;
    } else if ( ch >= 'A' && ch <= 'Z' ) {
        return ch + BASE32_BIG_ALPHA_OFFSET;
    } else if ( ch >= '2' && ch <= '7' ) {
        return ch + BASE32_NUMBER_OFFSET;
    } else {
        return - 1;
    }
}

extern inline
char * bt_base32_to_base16 ( TALLOC_CTX * ctx, char * encoded ) {
    size_t encoded_length = strlen ( encoded );
    size_t result_length  = ( size_t ) ceil ( 1.25 * encoded_length );
    char * result = talloc_array ( ctx, char, result_length );
    if ( !result ) {
        return NULL;
    }

    int8_t ch;
    char * walk     = encoded;
    char * walk_end = walk + encoded_length;

    uint32_t buffer       = 0;
    uint8_t  buffer_size  = 0;
    size_t   result_index = 0;
    while ( walk != walk_end ) {
        buffer <<= 5;
        ch = from_base32 ( *walk );
        if ( ch == -1 ) {
            talloc_free ( result );
            return NULL;
        }
        buffer |= ch;
        buffer_size += 5;

        if ( buffer_size >= 28 ) {
            while ( buffer_size ) {
                ch = ( char ) buffer & 0xF;
                //printf ( "%d\n", ( int ) ch );
                buffer >>= 4;
                result_index++;
                buffer_size -= 4;
            }
        }

        walk++;
    }

    return NULL;
}

#endif

