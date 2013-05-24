// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <string.h>

#include "magnet.h"
#include "btih.h"
#include "magnet/keylist.h"
#include "utils/url.h"

#include <talloc/tree.h>
#include <talloc/helpers.h>

static inline
uint8_t set_hash ( bt_magnet_info * info, char * value, size_t value_size )
{
    bt_hash * hash;
    if ( value_size == BT_HASH_BASE32_SRC ) {
        hash = bt_base32_decode ( info, value, BT_HASH_BASE32_SRC );
    } else if ( value_size == BT_HASH_BASE64_SRC ) {
        hash = bt_base64_decode ( info, value, BT_HASH_BASE64_SRC );
    } else {
        return 1;
    }

    if ( hash == NULL ) {
        return 2;
    }
    info->hash = hash;
    return 0;
}

static inline
uint8_t set_key_value ( bt_magnet_info * info, bt_magnet_keylist * trackers, char * key, size_t key_size, char * value, size_t value_size )
{
    if ( key_size == 2 ) {
        if ( !strncmp ( key, "xt", 2 ) && !strncmp ( value, "urn:btih:", 9 ) ) {
            // xt=urn:btih:${hash}
            value      += 9;
            value_size -= 9;
            if ( set_hash ( info, value, value_size ) ) {
                return 1;
            }
            return 0;
        } else if ( !strncmp ( key, "dn", 2 ) ) {
            // dn=${display_name}
            char * display_name = bt_unescape ( info, value, value_size );
            if ( display_name == NULL ) {
                return 2;
            }
            info->display_name = display_name;
            return 0;
        } else if ( !strncmp ( key, "ws", 2 ) ) {
            // ws=${webseed}
            char * link = bt_unescape ( info, value, value_size );
            if ( link == NULL ) {
                return 3;
            }
            if ( bt_list_append ( info->webseeds, link ) ) {
                return 4;
            }
            return 0;
        }
    }

    if ( !strncmp ( key, "tr", 2 ) ) {
        key      += 2;
        key_size -= 2;
        char * link = bt_unescape ( info, value, value_size );
        if ( link == NULL ) {
            return 5;
        }
        if ( bt_magnet_keylist_append ( trackers, key, key_size, link ) ) {
            return 6;
        }
        return 0;
    }

    return 7;
}

bt_magnet_info * bt_magnet_parse ( void * ctx, char * uri )
{
    if ( uri == NULL ) {
        return NULL;
    }
    if ( strncmp ( uri, "magnet:?", 8 ) ) {
        // not magnet link
        return NULL;
    }

    bt_magnet_info * info = talloc ( ctx, sizeof ( bt_magnet_info ) );
    if ( info == NULL ) {
        return NULL;
    }

    info->hash = NULL;
    info->display_name = NULL;

    info->trackers = bt_list_new ( info );
    if ( info->trackers == NULL ) {
        talloc_free ( info );
        return NULL;
    }
    bt_magnet_keylist * trackers = bt_magnet_keylist_new ( info->trackers );
    if ( trackers == NULL ) {
        talloc_free ( info );
        return NULL;
    }

    info->webseeds = bt_list_new ( info );
    if ( info->webseeds == NULL ) {
        talloc_free ( info );
        return NULL;
    }

    char * walk       = uri + 8;
    size_t uri_length = strlen ( uri );

    size_t key_size;
    size_t value_size;
    while ( walk ) {
        char * key   = walk;
        char * delim = strchr ( key, '=' );
        if ( delim == NULL ) {
            break;
        }
        key_size = delim - key;
        if ( key_size < 2 ) {
            talloc_free ( info );
            return NULL;
        }

        char * value = delim + 1;
        walk = strchr ( value, '&' );
        if ( walk == NULL ) {
            // the last value in uri. using the remaining bytes
            value_size = uri_length - ( value - uri );
        } else {
            value_size = walk - value;
            walk++;
        }

        if ( set_key_value ( info, trackers, key, key_size, value, value_size ) ) {
            talloc_free ( info );
            return NULL;
        }
    }

    if ( info->hash == NULL ) {
        // magnet without hash is invalid
        talloc_free ( info );
        return NULL;
    }

    talloc_free ( trackers );

    return info;
}

