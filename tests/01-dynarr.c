// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdint.h>

#include <talloc/tree.h>
#include <libbtr/utils/dynarr.h>

static bt_dynarr * arr;
static size_t a, b;

bool test_init ( void * ctx )
{
    arr = bt_dynarr_new ( ctx, 0 );
    if ( arr != NULL ) {
        return false;
    }
    arr = bt_dynarr_new ( ctx, 2 );
    if ( arr == NULL ) {
        return false;
    }
    return true;
}

bool test_append ()
{
    if (
        bt_dynarr_append ( arr, &a ) != 0 ||
        bt_dynarr_append ( arr, &a ) != 0 ||
        bt_dynarr_append ( arr, &b ) != 0 ||
        bt_dynarr_append ( arr, &a ) != 0 ||
        bt_dynarr_append ( arr, &a ) != 0 ||
        bt_dynarr_append ( arr, &b ) != 0 ||
        bt_dynarr_append ( arr, &a ) != 0 ||
        bt_dynarr_append ( arr, &b ) != 0 ||
        bt_dynarr_append ( arr, &b ) != 0
    ) {
        return false;
    }
    return true;
}

bool test_data ()
{
    bt_dynarr_set ( arr, 3, &b );
    bt_dynarr_set ( arr, 2, &a );

    if (
        bt_dynarr_get_length ( arr ) != 9 ||
        bt_dynarr_get ( arr, 0 ) != &a ||
        bt_dynarr_get ( arr, 1 ) != &a ||
        bt_dynarr_get ( arr, 2 ) != &a ||
        bt_dynarr_get ( arr, 3 ) != &b ||
        bt_dynarr_get ( arr, 4 ) != &a ||
        bt_dynarr_get ( arr, 5 ) != &b ||
        bt_dynarr_get ( arr, 6 ) != &a ||
        bt_dynarr_get ( arr, 7 ) != &b ||
        bt_dynarr_get ( arr, 8 ) != &b
    ) {
        return false;
    }
    return true;
}

int main ()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        talloc_free ( ctx );
        return 1;
    }

    if ( !test_init ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }
    if ( !test_append ( ctx ) ) {
        talloc_free ( ctx );
        return 3;
    }
    if ( !test_data ( ctx ) ) {
        talloc_free ( ctx );
        return 4;
    }

    talloc_free ( ctx );
    return 0;
}
