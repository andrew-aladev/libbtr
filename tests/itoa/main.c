// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <talloc2/tree.h>
#include <libbtr/utils/itoa.h>

bool test_number ( void * ctx, size_t number, char * answer )
{
    char * result = bt_size_t_to_str ( ctx, number );
    if ( result == NULL ) {
        return false;
    }
    if ( strcmp ( result, answer ) != 0 ) {
        talloc_free ( result );
        return false;
    }
    talloc_free ( result );
    return true;
}

bool test ( void * ctx )
{
    if (
        !test_number ( ctx, 0, "0" )           ||
        !test_number ( ctx, 123456, "123456" ) ||
        !test_number ( ctx, 987654, "987654" )
    ) {
        return false;
    }
    return true;
}

int main()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        talloc_free ( ctx );
        return 1;
    }

    if ( !test ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }

    talloc_free ( ctx );
    return 0;
}
