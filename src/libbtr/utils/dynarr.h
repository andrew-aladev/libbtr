// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#ifndef LIBBTR_UTILS_DYNARR_H
#define LIBBTR_UTILS_DYNARR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <talloc2/tree.h>

typedef struct bt_dynarr_t {
    size_t length;
    size_t start_capacity;
    size_t current_capacity;
    void ** data;
} bt_dynarr;

inline
bt_dynarr * bt_dynarr_new ( void * ctx, size_t capacity )
{
    if ( capacity == 0 ) {
        return NULL;
    }

    bt_dynarr * arr = talloc ( ctx, sizeof ( bt_dynarr ) );
    if ( arr == NULL ) {
        return NULL;
    }

    arr->start_capacity = arr->current_capacity = capacity;
    void ** data = talloc ( arr, arr->current_capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        talloc_free ( arr );
        return NULL;
    }
    arr->data = data;
    arr->length = 0;

    return arr;
}

inline
uint8_t bt_dynarr_grow ( bt_dynarr * arr )
{
    // linear growth
    arr->current_capacity = arr->current_capacity + arr->start_capacity;
    void ** reallocated_data = talloc_realloc ( arr->data, arr->current_capacity * sizeof ( uintptr_t ) );
    if ( reallocated_data == NULL ) {
        return 1;
    }
    arr->data = reallocated_data;
    return 0;
}

inline
uint8_t bt_dynarr_append ( bt_dynarr * arr, void * data )
{
    size_t index = arr->length;
    arr->length++;
    if ( arr->length > arr->current_capacity ) {
        if ( bt_dynarr_grow ( arr ) != 0 ) {
            return 1;
        }
    }
    arr->data[index] = data;
    return 0;
}

inline
void bt_dynarr_set ( bt_dynarr * arr, size_t position, void * pointer )
{
    arr->data[position] = pointer;
}

inline
void * bt_dynarr_get ( bt_dynarr * arr, size_t position )
{
    return arr->data[position];
}

inline
size_t bt_dynarr_get_length ( bt_dynarr * arr )
{
    return arr->length;
}

#endif

