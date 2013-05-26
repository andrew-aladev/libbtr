// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include "btih.h"

#include <talloc/tree.h>

typedef int8_t ( * convert_char ) ( int8_t ch );

static inline
int8_t from_base32 ( int8_t ch )
{
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
static inline
int8_t to_base32 ( int8_t ch )
{
    if ( ch >= 0 && ch <= 25 ) {
        return ch + 'a';
    } else if ( ch >= 26 && ch <= 31 ) {
        return ch + '2' - 26;
    } else {
        return - 1;
    }
}

static inline
int8_t from_base64 ( int8_t ch )
{
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
static inline
int8_t to_base64 ( int8_t ch )
{
    if ( ch >= 0 && ch <= 25 ) {
        return ch + 'A';
    } else if ( ch >= 26 && ch <= 51 ) {
        return ch - 26 + 'a';
    } else if ( ch >= 52 && ch <= 61 ) {
        return ch - 52 + '0';
    } else if ( ch == 62 ) {
        return '+';
    } else if ( ch == 63 ) {
        return '/';
    } else {
        return - 1;
    }
}

static inline
uint8_t decode ( bt_hash * hash, char * src, size_t src_length, uint8_t significant_bits, convert_char convert )
{
    size_t bits = src_length * significant_bits;
    if ( bits % 8 ) {
        // not properly padded string
        return 1;
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

        bits -= significant_bits;
    }

    if ( !bits ) {
        // all content is padding
        return 2;
    }

    size_t   length       = bits >> 3; // bits / 8
    uint32_t buffer       = 0;
    uint8_t  buffer_size  = 0;

    size_t result_index = 0;
    uint8_t * result = hash->binary = talloc ( hash, sizeof ( uint8_t ) * length );
    if ( result == NULL ) {
        return 3;
    }
    hash->length = length;

    while ( walk != walk_end ) {
        buffer <<= significant_bits;
        ch = convert ( * walk );
        if ( ch == -1 ) {
            return 4;
        }
        buffer |= ch;
        buffer_size += significant_bits;

        if ( buffer_size >= 8 ) {
            result[result_index] = ( uint8_t ) ( buffer >> ( buffer_size - 8 ) );
            buffer_size -= 8;
            result_index++;
        }

        walk++;
    }

    return 0;
}

static inline
size_t get_encoded_size ( bt_hash * hash, uint8_t significant_bits )
{
    size_t src_length = hash->length;
    size_t length = src_length / significant_bits;
    if ( src_length % significant_bits ) {
        // for padding
        length++;
    }
    return length << 3; // length * 3
}

static inline
uint8_t encode ( char * result, bt_hash * hash, size_t length, uint8_t significant_bits, convert_char convert )
{
    int8_t    ch;
    size_t    result_index = 0;
    uint8_t * walk             = hash->binary;
    uint8_t * walk_end         = hash->binary + hash->length;
    uint32_t  buffer           = 0;
    uint8_t   buffer_size      = 0;
    uint8_t   significant_mask = ( 1 << significant_bits ) - 1;

    while ( walk != walk_end ) {
        buffer <<= 8;
        buffer |= * walk;
        buffer_size += 8;

        while ( buffer_size >= significant_bits ) {
            ch = ( int8_t ) ( buffer >> ( buffer_size - significant_bits ) ) & significant_mask;
            ch = convert ( ch );
            if ( ch == -1 ) {
                return 1;
            }
            result[result_index] = ch;
            buffer_size -= significant_bits;
            result_index++;
        }

        walk++;
    }

    if ( buffer_size > 0 ) {
        ch = ( int8_t ) ( buffer << ( significant_bits - buffer_size ) ) & significant_mask;
        ch = convert ( ch );
        if ( ch == -1 ) {
            return 2;
        }
        result[result_index] = ch;
        result_index++;
    }

    while ( result_index < length ) {
        result[result_index] = '=';
        result_index++;
    }

    return 0;
}

static inline
bt_hash * _base_decode ( void * ctx, char * src, size_t src_length, uint8_t significant_bits, convert_char convert )
{
    if ( !src_length ) {
        return NULL;
    }

    bt_hash * hash = talloc ( ctx, sizeof ( bt_hash ) );
    if ( decode ( hash, src, src_length, significant_bits, convert ) ) {
        talloc_free ( hash );
        return NULL;
    }
    return hash;
}
bt_hash * bt_base32_decode ( void * ctx, char * src, size_t src_length )
{
    return _base_decode ( ctx, src, src_length, 5, from_base32 );
}
bt_hash * bt_base64_decode ( void * ctx, char * src, size_t src_length )
{
    return _base_decode ( ctx, src, src_length, 6, from_base64 );
}

static inline
char * base_encode ( void * ctx, bt_hash * hash, size_t * result_length, uint8_t significant_bits, convert_char convert )
{
    size_t length = get_encoded_size ( hash, significant_bits );
    if ( !length ) {
        return NULL;
    }
    char * result = talloc ( ctx, sizeof ( char ) * length + 1 );
    if ( result == NULL ) {
        return NULL;
    }
    * result_length = length;
    if ( encode ( result, hash, length, significant_bits, convert ) ) {
        talloc_free ( hash );
        return NULL;
    }
    result[length] = '\0';
    return result;
}
char * bt_base32_encode ( void * ctx, bt_hash * hash, size_t * result_length )
{
    return base_encode ( ctx, hash, result_length, 5, to_base32 );
}
char * bt_base64_encode ( void * ctx, bt_hash * hash, size_t * result_length )
{
    return base_encode ( ctx, hash, result_length, 6, to_base64 );
}
