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
    
    // should return NULL
    result = bt_base64_decode ( ctx, "", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 9;
    }
    result = bt_base64_decode ( ctx, "=", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 10;
    }
    result = bt_base64_decode ( ctx, "====", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 11;
    }
    result = bt_base64_decode ( ctx, "=a==", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 12;
    }
    result = bt_base64_decode ( ctx, "a====", &result_length );
    if ( result ) {
        talloc_free ( ctx );
        return 13;
    }
    
    // should return valid hash
    result = bt_base64_decode ( ctx, "YWJjZA==", &result_length );
    uint8_t answer_3[] = {'a', 'b', 'c', 'd', 0};
    if ( !result || result_length != 5 || memcmp ( result, answer_3, 4 ) ) {
        talloc_free ( ctx );
        return 14;
    }
    result = bt_base64_decode ( ctx, "7018b750d7be55ba6b05aca43dfea14c85502225", &result_length );
    uint8_t answer_4[] = { 0xEF, 0x4D, 0x7C, 0x6F, 0xBE, 0x74, 0x77, 0xB6, 0xDE, 0xE7, 0x96, 0xDA, 0xE9, 0xBD, 0x39, 0x69, 0xC6, 0xB8, 0xDD, 0xD7, 0xDE, 0x6B, 0x5E, 0x1C, 0xF3, 0x9E, 0x74, 0xDB, 0x6D, 0xB9};
    if ( !result || result_length != 30 || memcmp ( result, answer_4, 30 ) ) {
        talloc_free ( ctx );
        return 8;
    }

    talloc_free ( ctx );
    return 0;
}



