// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdint.h>

#include <talloc2/tree.h>
#include <libbtr/utils/list.h>

static bt_list * list;
static size_t a, b;

bool test_init ( void * ctx )
{
    list = bt_list_new ( ctx );
    if ( list == NULL ) {
        return false;
    }
    return true;
}

bool test_append ()
{
    if (
        bt_list_append ( list, &a ) != 0 ||
        bt_list_append ( list, &a ) != 0 ||
        bt_list_append ( list, &b ) != 0 ||
        bt_list_append ( list, &a ) != 0 ||
        bt_list_append ( list, &a ) != 0 ||
        bt_list_append ( list, &b ) != 0 ||
        bt_list_append ( list, &a ) != 0 ||
        bt_list_append ( list, &b ) != 0 ||
        bt_list_append ( list, &b ) != 0
    ) {
        return false;
    }
    return true;
}

bool test_data ()
{
    bt_list_item * item = list->last_item;
    if (
        bt_list_get_length ( list ) != 9 ||
        item == NULL ||

        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &a
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
