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
    uint8_t * result;
    size_t result_length = 0;

    // should return NULL
    result = bt_base32_decode ( ctx, "", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 2;
    }
    result = bt_base32_decode ( ctx, "=", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 3;
    }
    result = bt_base32_decode ( ctx, "========", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 4;
    }
    result = bt_base32_decode ( ctx, "=a======", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 5;
    }
    result = bt_base32_decode ( ctx, "a========", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 6;
    }

    // should return valid hash
    result = bt_base32_decode ( ctx, "MFRGG===", &result_length );
    uint8_t answer_1[] = {'a', 'b', 'c', 0};
    if ( !result || result_length != 4 || memcmp ( result, answer_1, 4 ) ) {
        talloc_free ( ctx );
        return 7;
    }
    result = bt_base32_decode ( ctx, "BGQHKHR56P6J32HGQTCCHFIA7UTHWEF6", &result_length );
    uint8_t answer_2[] = {0x09, 0xA0, 0x75, 0x1E, 0x3D, 0xF3, 0xFC, 0x9D, 0xE8, 0xE6, 0x84, 0xC4, 0x23, 0x95, 0x00, 0xFD, 0x26, 0x7B, 0x10, 0xBE, 0x00};
    if ( !result || result_length != 20 || memcmp ( result, answer_2, 20 ) ) {
        talloc_free ( ctx );
        return 8;
    }

    talloc_free ( ctx );
    return 0;
}



