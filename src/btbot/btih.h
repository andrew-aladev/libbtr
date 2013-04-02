// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

// See specification http://www.ietf.org/rfc/rfc4648.txt

#ifndef BTIH_H
#define BTIH_H

#include <stdint.h>
#include <talloc.h>
#include <inttypes.h>

#define __STDC_FORMAT_MACROS

static inline
int8_t from_base32 ( int8_t ch ) {
    if ( ch >= 'a' && ch <= 'z' ) {
        return ch - 'a';
    } else if ( ch >= 'A' && ch <= 'Z' ) {
        return ch - 'A';
    } else if ( ch >= '2' && ch <= '7' ) {
        return ch - '2' + 26;
    } else {
        return - 1;
    }
}

extern inline
uint8_t * bt_base32_decode ( TALLOC_CTX * ctx, char * src, size_t src_length, size_t * result_length ) {
    if ( !src_length ) {
        return NULL;
    }

    size_t bits = src_length * 5;
    if ( bits % 8 ) {
        // not properly padded string
        return NULL;
    }

    int8_t ch;
    char * walk     = src;
    char * walk_end = src + src_length;

    while ( walk != walk_end ) {
        walk_end--;
        ch = *walk_end;
        // bypass padding
        if ( ch != '=' ) {
            walk_end++;
            break;
        }

        bits -= 5;
    }

    if ( !bits ) {
        // all content is padding
        return NULL;
    }
    size_t length = bits / 8;
    if ( bits % 8 ) {
        // should add zeroes to the right
        length++;
    }

    uint32_t buffer       = 0;
    uint8_t  buffer_size  = 0;

    size_t result_index = 0;
    uint8_t * result    = talloc_array ( ctx, uint8_t, length );
    if ( !result ) {
        return NULL;
    }
    *result_length = length;

    while ( walk != walk_end ) {
        buffer <<= 5;
        ch = from_base32 ( *walk );
        if ( ch == -1 ) {
            talloc_free ( result );
            return NULL;
        }
        buffer |= ch;
        buffer_size += 5;

        if ( buffer_size >= 8 ) {
            result[result_index] = ( uint8_t ) ( buffer >> ( buffer_size - 8 ) );
            buffer_size -= 8;
            result_index++;
        }

        walk++;
    }

    if ( buffer_size ) {
        // adding zeroes to the right
        result[result_index] = ( uint8_t ) buffer << ( 8 - buffer_size );
    }

    return result;
}

static inline
int8_t from_base64 ( int8_t ch ) {
    if ( ch >= 'a' && ch <= 'z' ) {
        return ch - 'a' + 26;
    } else if ( ch >= 'A' && ch <= 'Z' ) {
        return ch - 'A';
    } else if ( ch >= '0' && ch <= '9' ) {
        return ch - '0' + 52;
    } else if ( ch == '+' ) {
        return 62;
    } else if ( ch == '/' ) {
        return 63;
    } else {
        return - 1;
    }
}

extern inline
uint8_t * bt_base64_decode ( TALLOC_CTX * ctx, char * src, size_t src_length, size_t * result_length ) {
    if ( !src_length ) {
        return NULL;
    }

    size_t bits = src_length * 6;
    if ( bits % 8 ) {
        // not properly padded string
        return NULL;
    }

    int8_t ch;
    char * walk     = src;
    char * walk_end = src + src_length;

    while ( walk != walk_end ) {
        walk_end--;
        ch = *walk_end;
        // bypass padding
        if ( ch != '=' ) {
            walk_end++;
            break;
        }

        bits -= 6;
    }

    if ( !bits ) {
        // all content is padding
        return NULL;
    }
    size_t length = bits / 8;
    if ( bits % 8 ) {
        // should add zeroes to the right
        length++;
    }

    uint32_t buffer       = 0;
    uint8_t  buffer_size  = 0;

    size_t result_index = 0;
    uint8_t * result    = talloc_array ( ctx, uint8_t, length );
    if ( !result ) {
        return NULL;
    }
    *result_length = length;

    while ( walk != walk_end ) {
        buffer <<= 6;
        ch = from_base64 ( *walk );
        if ( ch == -1 ) {
            talloc_free ( result );
            return NULL;
        }
        buffer |= ch;
        buffer_size += 6;

        if ( buffer_size >= 8 ) {
            result[result_index] = ( uint8_t ) ( buffer >> ( buffer_size - 8 ) );
            buffer_size -= 8;
            result_index++;
        }

        walk++;
    }

    if ( buffer_size ) {
        // adding zeroes to the right
        result[result_index] = ( uint8_t ) buffer << ( 8 - buffer_size );
    }

    return result;
}

#endif

