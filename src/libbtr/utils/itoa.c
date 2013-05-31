// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include "itoa.h"

#include <math.h>

#include <talloc2/tree.h>

char * bt_size_t_to_str ( void * ctx, size_t number )
{
    char * result;

    if ( number == 0 ) {
        result = talloc ( ctx, sizeof ( char ) * 2 );
        if ( result == NULL ) {
            return NULL;
        }
        result[0] = '0';
        result[1] = '\0';
        return result;
    }

    // size of number + 1 for '\0'
    uint8_t length = ( uint8_t ) floor ( log10 ( number ) ) + 2;
    result = talloc ( ctx, sizeof ( char ) * length );
    if ( result == NULL ) {
        return NULL;
    }
    result[length - 1] = '\0';
    char * walk        = result + length - 1;

    uint8_t digit;
    while ( number > 0 ) {
        walk--;
        digit  = number % 10;
        number /= 10;
        * walk = '0' + digit;
    }

    return result;
}
