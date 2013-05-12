// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <string.h>

#include "magnet.h"
#include "btih.h"
#include "utils/dynarr.h"
#include "utils/url.h"
#include "utils/itoa.h"

#include <talloc/tree.h>
#include <talloc/helpers.h>

static inline
uint8_t set_hash ( bt_magnet_info * info, char * value, size_t value_size ) {
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
uint8_t set_display_name ( bt_magnet_info * info, char * value, size_t value_size ) {
    char * display_name = talloc_strndup ( info, value, value_size );
    if ( display_name == NULL ) {
        return 1;
    }
    char * display_name_escaped = bt_unescape ( info, display_name );
    talloc_free ( display_name );
    if ( display_name_escaped == NULL ) {
        return 2;
    }

    info->display_name = display_name_escaped;
    return 0;
}

static inline
uint8_t append_link_to_list ( bt_magnet_info * info, bt_dynarr * list, char * value, size_t value_size ) {
    char * link = talloc_strndup ( info, value, value_size );
    if ( link == NULL ) {
        return 1;
    }
    char * escaped_link = bt_unescape ( info, link );
    talloc_free ( link );
    if ( escaped_link == NULL ) {
        return 2;
    }
    if ( bt_dynarr_append ( list, escaped_link ) ) {
        return 3;
    }
    return 0;
}

static inline
uint8_t set_key_value ( bt_magnet_info * info, char * key, size_t key_size, char * value, size_t value_size, size_t * next_tracker_index, uint8_t * trackers_mode ) {
    if ( key_size == 2 ) {
        if ( !strncmp ( key, "xt", 2 ) && !strncmp ( value, "urn:btih:", 9 ) ) {
            // xt=urn:btih:${hash}
            value      += 9;
            value_size -= 9;
            if ( set_hash ( info, value, value_size ) ) {
                return 1;
            }
        } else if ( !strncmp ( key, "dn", 2 ) ) {
            // dn=${display_name}
            if ( set_display_name ( info, value, value_size ) ) {
                return 2;
            }
        } else if ( !strncmp ( key, "tr", 2 ) ) {
            // tr=${tracker}
            size_t mode = * trackers_mode;
            if ( mode == 2 ) {
                return 3;
            } else if ( !mode ) {
                * trackers_mode = 1;
            }
            if ( append_link_to_list ( info, info->trackers, value, value_size ) ) {
                return 3;
            }
        } else if ( !strncmp ( key, "ws", 2 ) ) {
            // ws=${webseed}
            if ( append_link_to_list ( info, info->webseeds, value, value_size ) ) {
                return 4;
            }
        }
    } else {
        if ( !strncmp ( key, "tr.", 3 ) ) {
            // tr.${tr_i}=${tracker}
            // both (tr.0, tr.1, ...) and (tr.1, tr.2, ...) are allowed
            size_t mode = * trackers_mode;
            if ( mode == 1 ) {
                return 3;
            } else if ( !mode ) {
                * trackers_mode = 2;
            }

            key      += 3;
            key_size -= 3;

            size_t trackers_length = bt_dynarr_get_length ( info->trackers );
            if ( trackers_length ) {
                char * tracker_index = bt_size_t_to_str ( info, * next_tracker_index );
                if ( tracker_index == NULL ) {
                    return 5;
                }
                if ( key_size == strlen ( tracker_index ) && !strncmp ( key, tracker_index, key_size ) ) {
                    talloc_free ( tracker_index );
                    if ( append_link_to_list ( info, info->trackers, value, value_size ) ) {
                        return 5;
                    }
                    ( * next_tracker_index ) ++;
                } else {
                    // unknown key
                    talloc_free ( tracker_index );
                    return 5;
                }
            } else {
                if ( key_size == 1 ) {
                    char first_char_of_key = * key;

                    if ( first_char_of_key == '0' ) {
                        if ( append_link_to_list ( info, info->trackers, value, value_size ) ) {
                            return 5;
                        }
                        * next_tracker_index = 1;
                    } else if ( first_char_of_key == '1' ) {
                        if ( append_link_to_list ( info, info->trackers, value, value_size ) ) {
                            return 6;
                        }
                        * next_tracker_index = 2;
                    } else {
                        // unknown key
                        return 7;
                    }
                } else {
                    // unknown key
                    return 8;
                }
            }
        } else {
            return 7;
        }
    }

    return 0;
}

bt_magnet_info * bt_magnet_parse ( void * ctx, char * uri ) {
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

    // spec has no limit to trackers count
    info->trackers = bt_dynarr_new ( info, 2 );
    if ( info->trackers == NULL ) {
        talloc_free ( info );
        return NULL;
    }
    // spec has no limit to webseeds count
    info->webseeds = bt_dynarr_new ( info, 2 );
    if ( info->webseeds == NULL ) {
        talloc_free ( info );
        return NULL;
    }

    char * walk       = uri + 8;
    size_t uri_length = strlen ( uri );

    uint8_t trackers_mode      = 0;
    size_t  next_tracker_index = 0;

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

        if ( set_key_value ( info, key, key_size, value, value_size, & next_tracker_index, & trackers_mode ) ) {
            talloc_free ( info );
            return NULL;
        }
    }

    if ( info->hash == NULL ) {
        // magnet without hash is invalid
        talloc_free ( info );
        return NULL;
    }

    return info;
}

