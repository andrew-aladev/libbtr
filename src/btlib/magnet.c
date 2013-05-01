// This file is part of btlib. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btlib is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btlib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btlib. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <string.h>
#include <talloc.h>
#include "magnet.h"
#include "utils/list.h"
#include "utils/url.h"
#include "utils/itoa.h"
#include "btih.h"

static inline
bool set_hash ( bt_magnet_info * info, char * value, size_t value_size ) {
    if ( value_size == BT_HASH_BASE32_SRC + 9 ) {
        info->hash = bt_base32_decode ( info, value + 9, BT_HASH_BASE32_SRC, & info->hash_length );
    } else if ( value_size == BT_HASH_BASE64_SRC + 9 ) {
        info->hash = bt_base64_decode ( info, value + 9, BT_HASH_BASE64_SRC, & info->hash_length );
    }

    if ( !info->hash ) {
        return false;
    }
    return true;
}

static inline
bool set_display_name ( bt_magnet_info * info, char * value, size_t value_size ) {
    char * display_name = talloc_strndup ( info, value, value_size );
    if ( !display_name ) {
        return false;
    }
    char * display_name_escaped = bt_unescape ( info, display_name );
    talloc_free ( display_name );
    if ( !display_name_escaped ) {
        return false;
    }

    info->display_name = display_name_escaped;
    return true;
}

static inline
bool append_link_to_list ( bt_magnet_info * info, bt_list * list, char * value, size_t value_size ) {
    char * link = talloc_strndup ( info, value, value_size );
    if ( !link ) {
        return false;
    }
    char * escaped_link = bt_unescape ( info, link );
    talloc_free ( link );
    if ( !escaped_link ) {
        return false;
    }
    if ( !bt_list_append ( list, escaped_link ) ) {
        return false;
    }
    return true;
}

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

    TALLOC_CTX * trackers_ctx = talloc_new ( info );
    if ( !trackers_ctx ) {
        talloc_free ( info );
        return NULL;
    }
    TALLOC_CTX * webseeds_ctx = talloc_new ( info );
    if ( !webseeds_ctx ) {
        talloc_free ( info );
        return NULL;
    }

    // spec has no limit to trackers count
    bt_list * trackers_list = bt_list_create ( trackers_ctx );
    if ( !trackers_list ) {
        talloc_free ( info );
        return NULL;
    }
    // spec has no limit to webseeds count
    bt_list * webseeds_list = bt_list_create ( webseeds_ctx );
    if ( !webseeds_list ) {
        talloc_free ( info );
        return NULL;
    }

    char * walk       = uri + 8;
    size_t uri_length = strlen ( uri );

    bool has_tr   = false;
    bool has_hash = false;
    bool has_tr_d = false;
    size_t tr_d;

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

        if ( key_size == 2 ) {
            if ( !strncmp ( key, "xt", 2 ) && !strncmp ( value, "urn:btih:", 9 ) ) {
                // xt=urn:btih:${hash}

                if ( !set_hash ( info, value, value_size ) ) {
                    talloc_free ( info );
                    return NULL;
                }
                has_hash = true;

            } else if ( !strncmp ( key, "dn", 2 ) ) {
                // dn=${display_name}

                if ( !set_display_name ( info, value, value_size ) ) {
                    talloc_free ( info );
                    return NULL;
                }
            } else if ( !strncmp ( key, "tr", 2 ) ) {
                // tr=${tracker}

                if ( !append_link_to_list ( info, trackers_list, value, value_size ) ) {
                    talloc_free ( info );
                    return NULL;
                }
                has_tr = true;
            } else if ( !strncmp ( key, "ws", 2 ) ) {
                // ws=${webseed}

                if ( !append_link_to_list ( info, webseeds_list, value, value_size ) ) {
                    talloc_free ( info );
                    return NULL;
                }
            }
        } else if ( key_size > 2 ) {
            if ( !strncmp ( key, "tr.", 3 ) ) {
                // tr.${tr_i}=${tracker}

                key      += 3;
                key_size -= 3;

                if ( !has_tr_d ) {
                    // both (tr.0, tr.1, ...) and (tr.1, tr.2, ...) are allowed

                    if ( key_size == 1 && *key == '0' ) {
                        if ( !append_link_to_list ( info, trackers_list, value, value_size ) ) {
                            talloc_free ( info );
                            return NULL;
                        }
                        tr_d     = 1;
                        has_tr_d = true;
                    } else if ( key_size == 1 && *key == '1' ) {
                        if ( !append_link_to_list ( info, trackers_list, value, value_size ) ) {
                            talloc_free ( info );
                            return NULL;
                        }
                        tr_d     = 2;
                        has_tr_d = true;
                    } else {
                        // unknown key
                        talloc_free ( info );
                        return NULL;
                    }
                } else {
                    char * tr_i = bt_size_t_to_str ( trackers_ctx, tr_d );
                    if ( !tr_i ) {
                        talloc_free ( info );
                        return NULL;
                    }

                    if ( key_size == strlen ( tr_i ) && !strncmp ( key, tr_i, key_size ) ) {
                        talloc_free ( tr_i );

                        if ( !append_link_to_list ( info, trackers_list, value, value_size ) ) {
                            talloc_free ( info );
                            return NULL;
                        }
                        tr_d++;
                    } else {
                        // unknown key
                        talloc_free ( tr_i );
                        talloc_free ( info );
                        return NULL;
                    }
                }
            }
        }
    }

    if ( !has_hash ) {
        // magnet link without hash is invalid
        talloc_free ( info );
        return NULL;
    }
    if ( has_tr && has_tr_d ) {
        // magnet link with mixed tr and tr.d is invalid
        talloc_free ( info );
        return NULL;
    }

    size_t trackers_count = bt_list_get_length ( trackers_list );
    size_t webseeds_count = bt_list_get_length ( webseeds_list );
    info->trackers_count  = trackers_count;
    info->webseeds_count  = webseeds_count;

    info->trackers = talloc_array ( info, char *, trackers_count );
    if ( !info->trackers ) {
        talloc_free ( info );
        return NULL;
    }
    info->webseeds = talloc_array ( info, char *, webseeds_count );
    if ( !info->trackers ) {
        talloc_free ( info );
        return NULL;
    }
    if ( !bt_list_data_copy ( trackers_list, ( void ** ) info->trackers, trackers_count ) ) {
        talloc_free ( info );
        return NULL;
    }
    if ( !bt_list_data_copy ( webseeds_list, ( void ** ) info->webseeds, webseeds_count ) ) {
        talloc_free ( info );
        return NULL;
    }
    talloc_free ( trackers_ctx );
    talloc_free ( webseeds_ctx );

    return info;
}
