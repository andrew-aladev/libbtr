// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <string.h>
#include <talloc.h>
#include "magnet.h"
#include "list.h"
#include "url.h"
#include "btih.h"

bt_magnet_info * bt_magnet_parse ( TALLOC_CTX * ctx, char * uri ) {
    if ( !uri ) {
        return NULL;
    }
    if ( strncmp ( uri, "magnet:?", 8 ) ) {
        // not magnet link
        return NULL;
    }

    bt_magnet_info * info = talloc ( ctx, bt_magnet_info );
    if ( !info ) {
        return NULL;
    }

    TALLOC_CTX * trackers_ctx = talloc_new ( ctx );
    if ( !trackers_ctx ) {
        talloc_free ( info );
        return NULL;
    }

    // spec has no limit to trackers count
    bt_list * trackers_list = bt_list_create ( trackers_ctx );
    if ( !trackers_list ) {
        talloc_free ( info );
        talloc_free ( trackers_ctx );
        return NULL;
    }

    char * walk       = uri + 8;
    size_t uri_length = strlen ( uri );
    bool has_hash     = false;

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
            talloc_free ( trackers_ctx );
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

        if ( key_size == 2 ) {
            if ( !strncmp ( key, "xt", 2 ) && !strncmp ( value, "urn:btih:", 9 ) ) {
                if ( value_size == BT_HASH_BASE32_SRC + 9 ) {
                    info->hash = bt_base32_decode ( info, value + 9, BT_HASH_BASE32_SRC, & info->hash_length );
                } else if ( value_size == BT_HASH_BASE64_SRC + 9 ) {
                    info->hash = bt_base64_decode ( info, value + 9, BT_HASH_BASE64_SRC, & info->hash_length );
                }

                if ( !info->hash ) {
                    talloc_free ( info );
                    talloc_free ( trackers_ctx );
                    return NULL;
                }
                has_hash = true;

            } else if ( !strncmp ( key, "dn", 2 ) ) {
                char * display_name = talloc_strndup ( info, value, value_size );
                if ( !display_name ) {
                    talloc_free ( info );
                    talloc_free ( trackers_ctx );
                    return NULL;
                }
                char * display_name_escaped = bt_unescape ( info, display_name );
                talloc_free ( display_name );
                if ( !display_name_escaped ) {
                    talloc_free ( info );
                    talloc_free ( trackers_ctx );
                    return NULL;
                }
                info->display_name = display_name_escaped;
            }
        }

        if ( !strncmp ( key, "tr", 2 ) ) {
            // accepting tr and tr* (like tr.1, tr.2 etc)

            char * tracker = talloc_strndup ( info, value, value_size );
            if ( !tracker ) {
                talloc_free ( info );
                talloc_free ( trackers_ctx );
                return NULL;
            }
            char * tracker_escaped = bt_unescape ( info, tracker );
            talloc_free ( tracker );
            if ( !tracker_escaped ) {
                talloc_free ( info );
                talloc_free ( trackers_ctx );
                return NULL;
            }
            if ( !bt_list_append ( trackers_list, tracker_escaped ) ) {
                talloc_free ( info );
                talloc_free ( trackers_ctx );
                return NULL;
            }
        }
    }

    size_t trackers_length = bt_list_get_length ( trackers_list );
    info->trackers_count   = trackers_length;
    info->trackers         = talloc_array ( info, char *, trackers_length );
    if ( !info->trackers ) {
        talloc_free ( info );
        talloc_free ( trackers_ctx );
        return NULL;
    }
    // obtain usual array from list
    if ( !bt_list_data_copy ( trackers_list, ( void ** ) info->trackers, trackers_length ) ) {
        talloc_free ( info );
        talloc_free ( trackers_ctx );
        return NULL;
    }
    talloc_free ( trackers_ctx );

    if ( !has_hash ) {
        talloc_free ( info );
        return NULL;
    }

    return info;
}
