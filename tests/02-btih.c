// This file is part of btbot. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btbot is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btbot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with btbot. If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <talloc.h>
#include <btbot/btih.h>

int main() {
    TALLOC_CTX * ctx = talloc_new ( NULL );
    if ( !ctx ) {
        talloc_free ( ctx );
        return 1;
    }
    bt_base32_to_base16 ( ctx, "abcdefghijklmnopqrstuvwxqz234567" );

    talloc_free ( ctx );
    return 0;
}


