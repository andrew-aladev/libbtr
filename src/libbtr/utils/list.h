// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#ifndef LIBBTR_UTILS_LIST_H
#define LIBBTR_UTILS_LIST_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <talloc/tree.h>
#include <talloc/ext.h>

typedef struct bt_list_item_t {
    struct bt_list_item_t * prev;
    void * data;
} bt_list_item;

typedef struct bt_list_t {
    struct bt_list_item_t * last_item;
    size_t length;
} bt_list;

static
void bt_list_free ( void * current_list ) {
    bt_list * list      = current_list;
    bt_list_item * item = list->last_item;
    bt_list_item * prev_item;
    while ( item != NULL ) {
        prev_item = item->prev;
        free ( item );
        item = prev_item;
    }
}

inline
bt_list * bt_list_new ( void * ctx ) {
    bt_list * list = talloc ( ctx, sizeof ( bt_list ) );
    if ( list == NULL ) {
        return NULL;
    }
    talloc_set_destructor ( list, bt_list_free );

    list->last_item = NULL;
    list->length    = 0;
    return list;
}

inline
uint8_t bt_list_append ( bt_list * list, void * data ) {
    bt_list_item * item = malloc ( sizeof ( bt_list_item ) );
    if ( item == NULL ) {
        return 1;
    }
    item->data = data;
    item->prev = list->last_item;
    
    list->last_item = item;
    list->length++;
    return 0;
}

inline
size_t bt_list_get_length ( bt_list * list ) {
    return list->length;
}

#endif
