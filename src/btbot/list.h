// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <talloc.h>

// use this list
// 1. create new list
// 2. append pointers
// 3. copy list to array
// 4. delete list

typedef struct bt_list_node {
    void * data;
    struct bt_list_node * next;
} bt_list_node;

typedef struct bt_list {
    bt_list_node * first;
    bt_list_node * last;
    size_t length;
} bt_list;

extern inline
bt_list * bt_list_create ( TALLOC_CTX * ctx ) {
    bt_list * list = talloc ( ctx, bt_list );
    if ( !list ) {
        return NULL;
    }

    list->first  = NULL;
    list->last   = NULL;
    list->length = 0;
    return list;
}

extern inline
bool bt_list_append ( bt_list * list, void * data ) {
    bt_list_node * node = talloc ( list, bt_list_node );
    if ( !node ) {
        return false;
    }

    node->data = data;
    node->next = NULL;
    if ( !list->last ) {
        list->length = 1;
        list->first  = node;
        list->last   = node;
    } else {
        list->last->next = node;
        list->last       = node;
        list->length     = list->length + 1;
    }

    return true;
}

extern inline
size_t bt_list_get_length ( bt_list * list ) {
    return list->length;
}

extern inline
bool bt_list_data_copy ( bt_list * list, void ** array, size_t length ) {
    size_t i;
    bt_list_node * node = list->first;
    for ( i = 0; i < length; i++ ) {
        if ( !node ) {
            return false;
        }
        array[i] = node->data;
        node     = node->next;
    }
    return true;
}

#endif
