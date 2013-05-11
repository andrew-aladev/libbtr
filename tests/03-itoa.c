// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <talloc/tree.h>
#include <talloc/helpers.h>
#include <libbtr/utils/itoa.h>

int main() {
    void * ctx = talloc_new ( NULL );
    if ( !ctx ) {
        talloc_free ( ctx );
        return 1;
    }
    char * str;

    str = bt_size_t_to_str ( ctx, 0 );
    if ( strcmp ( str, "0" ) ) {
        return 1;
    }
    talloc_free ( str );

    str = bt_size_t_to_str ( ctx, 123456 );
    if ( strcmp ( str, "123456" ) ) {
        return 2;
    }
    talloc_free ( str );

    talloc_free ( ctx );
    return 0;
}


