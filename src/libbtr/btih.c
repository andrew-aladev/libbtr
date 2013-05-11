#include "btih.h"

#include <talloc/tree.h>

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

uint8_t * bt_base32_decode ( void * ctx, char * src, size_t src_length, size_t * result_length ) {
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

    size_t length       = bits / 8;
    size_t result_index = 0;
    uint8_t * result    = talloc ( ctx, sizeof ( uint8_t ) * length );
    if ( !result ) {
        return NULL;
    }
    *result_length = length;

    uint32_t buffer       = 0;
    uint8_t  buffer_size  = 0;
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

    return result;
}

static inline
int8_t to_base32 ( int8_t ch ) {
    if ( ch >= 0 && ch <= 25 ) {
        return ch + 'a';
    } else if ( ch >= 26 && ch <= 31 ) {
        return ch + '2' - 26;
    } else {
        return - 1;
    }
}

char * bt_base32_encode ( void * ctx, uint8_t * src, size_t src_length, size_t * result_length ) {
    if ( !src_length ) {
        return NULL;
    }

    size_t length = src_length / 5;
    if ( src_length % 5 ) {
        // for padding
        length++;
    }
    length = length * 8 + 1;

    size_t result_index = 0;
    char * result       = talloc ( ctx, sizeof ( char ) * length );
    if ( !result ) {
        return NULL;
    }
    *result_length = length;

    int8_t    ch;
    uint8_t * walk         = src;
    uint8_t * walk_end     = src + src_length;
    uint32_t  buffer       = 0;
    uint8_t   buffer_size  = 0;
    while ( walk != walk_end ) {
        buffer <<= 8;
        buffer |= *walk;
        buffer_size += 8;

        while ( buffer_size >= 5 ) {
            ch = ( int8_t ) ( buffer >> ( buffer_size - 5 ) ) & 0x1F;
            ch = to_base32 ( ch );
            if ( ch == -1 ) {
                talloc_free ( result );
                return NULL;
            }
            result[result_index] = ch;
            buffer_size -= 5;
            result_index++;
        }

        walk++;
    }

    if ( buffer_size > 0 ) {
        ch = ( int8_t ) ( buffer << ( 5 - buffer_size ) ) & 0x1F;
        ch = to_base32 ( ch );
        if ( ch == -1 ) {
            talloc_free ( result );
            return NULL;
        }
        result[result_index] = ch;
        result_index++;
    }

    while ( result_index < length - 1 ) {
        result[result_index] = '=';
        result_index++;
    }
    result[result_index] = '\0';

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

uint8_t * bt_base64_decode ( void * ctx, char * src, size_t src_length, size_t * result_length ) {
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

    size_t   length       = bits / 8;
    uint32_t buffer       = 0;
    uint8_t  buffer_size  = 0;

    size_t result_index = 0;
    uint8_t * result    = talloc ( ctx, sizeof ( uint8_t ) * length );
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

    return result;
}

static inline
int8_t to_base64 ( int8_t ch ) {
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

char * bt_base64_encode ( void * ctx, uint8_t * src, size_t src_length, size_t * result_length ) {
    if ( !src_length ) {
        return NULL;
    }

    size_t length = src_length / 6;
    if ( src_length % 6 ) {
        // for padding
        length++;
    }
    length = length * 8 + 1;

    size_t result_index = 0;
    char * result       = talloc ( ctx, sizeof ( char ) * length );
    if ( !result ) {
        return NULL;
    }
    *result_length = length;

    int8_t    ch;
    uint8_t * walk         = src;
    uint8_t * walk_end     = src + src_length;
    uint32_t  buffer       = 0;
    uint8_t   buffer_size  = 0;
    while ( walk != walk_end ) {
        buffer <<= 8;
        buffer |= *walk;
        buffer_size += 8;

        while ( buffer_size >= 6 ) {
            ch = ( int8_t ) ( buffer >> ( buffer_size - 6 ) ) & 0x3F;
            ch = to_base64 ( ch );
            if ( ch == -1 ) {
                talloc_free ( result );
                return NULL;
            }
            result[result_index] = ch;
            buffer_size -= 6;
            result_index++;
        }

        walk++;
    }

    if ( buffer_size > 0 ) {
        ch = ( int8_t ) ( buffer << ( 6 - buffer_size ) ) & 0x3F;
        ch = to_base64 ( ch );
        if ( ch == -1 ) {
            talloc_free ( result );
            return NULL;
        }
        result[result_index] = ch;
        result_index++;
    }

    while ( result_index < length - 1 ) {
        result[result_index] = '=';
        result_index++;
    }
    result[result_index] = '\0';

    return result;
}
