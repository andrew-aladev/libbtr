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

/*
int base32_decode ( const uint8_t *encoded, uint8_t *result, int bufSize ) {
    int buffer = 0;
    int bitsLeft = 0;
    int count = 0;
    for ( const uint8_t *ptr = encoded; count < bufSize && *ptr; ++ptr ) {
        uint8_t ch = *ptr;
        buffer <<= 5;

        // Look up one base32 digit
        if ( ( ch >= 'A' && ch <= 'Z' ) || ( ch >= 'a' && ch <= 'z' ) ) {
            ch = ( ch & 0x1F ) - 1;
        } else if ( ch >= '2' && ch <= '7' ) {
            ch -= '2' - 26;
        } else {
            return -1;
        }

        buffer |= ch;
        bitsLeft += 5;
        if ( bitsLeft >= 8 ) {
            result[count++] = buffer >> ( bitsLeft - 8 );
            bitsLeft -= 8;
        }
    }
    if ( count < bufSize ) {
        result[count] = '\000';
    }
    return count;
}
*/

#endif
