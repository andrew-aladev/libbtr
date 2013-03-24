#ifndef URL_H
#define URL_H

#include <stdbool.h>
#include <string.h>
#include <talloc.h>

// see http://www.ietf.org/rfc/rfc1738.txt
extern inline
char * bt_unescape ( TALLOC_CTX * ctx, char * url ) {
    if ( !url ) {
        return NULL;
    }

    size_t url_length = strlen ( url );
    char * buffer     = talloc_array ( ctx, char, url_length + 1 );
    if ( !buffer ) {
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
                talloc_free ( buffer );
                return NULL;
            }
            
            char * hex = talloc_strndup ( buffer, walk_url, 2 );
            if ( !hex ) {
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
    talloc_free ( buffer );
    return result;
}

#endif

