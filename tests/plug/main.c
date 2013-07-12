// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/tree.h>

#include "address.h"

int main()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        talloc_free ( ctx );
        return 1;
    }

    if ( !test_address ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }

    talloc_free ( ctx );
    return 0;
}
