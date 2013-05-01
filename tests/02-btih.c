// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <talloc.h>
#include <libbtr/btih.h>

int main() {
    TALLOC_CTX * ctx = talloc_new ( NULL );
    if ( !ctx ) {
        talloc_free ( ctx );
        return 1;
    }
    char *    src_string;
    uint8_t * result_hash;
    size_t    result_length = 0;

    // should return NULL
    src_string = "";
    result_hash = bt_base32_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 2;
    }
    src_string = "=";
    result_hash = bt_base32_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 3;
    }
    src_string = "========";
    result_hash = bt_base32_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 4;
    }
    src_string = "=a======";
    result_hash = bt_base32_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 5;
    }
    src_string = "a========";
    result_hash = bt_base32_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 6;
    }

    // should return valid hash
    src_string = "MFRGG===";
    result_hash = bt_base32_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    uint8_t answer_1[] = {'a', 'b', 'c'};
    if ( !result_hash || result_length != sizeof ( answer_1 ) || memcmp ( result_hash, answer_1, sizeof ( answer_1 ) ) ) {
        talloc_free ( ctx );
        return 7;
    }
    talloc_free ( result_hash );

    src_string = "BGQHKHR56P6J32HGQTCCHFIA7UTHWEF6";
    result_hash = bt_base32_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    uint8_t answer_2[] = {0x09, 0xA0, 0x75, 0x1E, 0x3D, 0xF3, 0xFC, 0x9D, 0xE8, 0xE6, 0x84, 0xC4, 0x23, 0x95, 0x00, 0xFD, 0x26, 0x7B, 0x10, 0xBE};
    if ( !result_hash || result_length != sizeof ( answer_2 ) || memcmp ( result_hash, answer_2, sizeof ( answer_2 ) ) ) {
        talloc_free ( ctx );
        return 8;
    }
    talloc_free ( result_hash );

    // should return NULL
    src_string = "";
    result_hash = bt_base64_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 9;
    }
    src_string = "=";
    result_hash = bt_base64_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 10;
    }
    src_string = "====";
    result_hash = bt_base64_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 11;
    }
    src_string = "=a==";
    result_hash = bt_base64_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 12;
    }
    src_string = "a====";
    result_hash = bt_base64_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    if ( result_hash ) {
        talloc_free ( ctx );
        return 13;
    }

    // should return valid hash
    src_string = "YWJjZA==";
    result_hash = bt_base64_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    uint8_t answer_3[] = {'a', 'b', 'c', 'd'};
    if ( !result_hash || result_length != sizeof ( answer_3 ) || memcmp ( result_hash, answer_3, sizeof ( answer_3 ) ) ) {
        talloc_free ( ctx );
        return 14;
    }
    talloc_free ( result_hash );

    src_string = "7018b750d7be55ba6b05aca43dfea14c85502225";
    result_hash = bt_base64_decode ( ctx, src_string, strlen ( src_string ), &result_length );
    uint8_t answer_4[] = { 0xEF, 0x4D, 0x7C, 0x6F, 0xBE, 0x74, 0x77, 0xB6, 0xDE, 0xE7, 0x96, 0xDA, 0xE9, 0xBD, 0x39, 0x69, 0xC6, 0xB8, 0xDD, 0xD7, 0xDE, 0x6B, 0x5E, 0x1C, 0xF3, 0x9E, 0x74, 0xDB, 0x6D, 0xB9};
    if ( !result_hash || result_length != sizeof ( answer_4 ) || memcmp ( result_hash, answer_4, sizeof ( answer_4 ) ) ) {
        talloc_free ( ctx );
        return 15;
    }
    talloc_free ( result_hash );

    // should return valid string
    char * result_string;

    uint8_t src_hash_1[] = {'a', 'b', 'c'};
    result_string = bt_base32_encode ( ctx, src_hash_1, sizeof ( src_hash_1 ), &result_length );
    if ( !result_string || result_length != 9 || strcmp ( result_string, "mfrgg===" ) ) {
        talloc_free ( ctx );
        return 16;
    }
    talloc_free ( result_string );

    uint8_t src_hash_2[] = {0x09, 0xA0, 0x75, 0x1E, 0x3D, 0xF3, 0xFC, 0x9D, 0xE8, 0xE6, 0x84, 0xC4, 0x23, 0x95, 0x00, 0xFD, 0x26, 0x7B, 0x10, 0xBE};
    result_string = bt_base32_encode ( ctx, src_hash_2, sizeof ( src_hash_2 ), &result_length );
    if ( !result_string || result_length != 33 || strcmp ( result_string, "bgqhkhr56p6j32hgqtcchfia7uthwef6" ) ) {
        talloc_free ( ctx );
        return 17;
    }
    talloc_free ( result_string );

    uint8_t src_hash_3[] = {'a', 'b', 'c', 'd'};
    result_string = bt_base64_encode ( ctx, src_hash_3, sizeof ( src_hash_3 ), &result_length );
    if ( !result_string || result_length != 9 || strcmp ( result_string, "YWJjZA==" ) ) {
        talloc_free ( ctx );
        return 18;
    }
    talloc_free ( result_string );
    
    uint8_t src_hash_4[] = { 0xEF, 0x4D, 0x7C, 0x6F, 0xBE, 0x74, 0x77, 0xB6, 0xDE, 0xE7, 0x96, 0xDA, 0xE9, 0xBD, 0x39, 0x69, 0xC6, 0xB8, 0xDD, 0xD7, 0xDE, 0x6B, 0x5E, 0x1C, 0xF3, 0x9E, 0x74, 0xDB, 0x6D, 0xB9};
    result_string = bt_base64_encode ( ctx, src_hash_4, sizeof ( src_hash_4 ), &result_length );
    if ( !result_string || result_length != 41 || strcmp ( result_string, "7018b750d7be55ba6b05aca43dfea14c85502225" ) ) {
        talloc_free ( ctx );
        return 19;
    }
    talloc_free ( result_string );

    talloc_free ( ctx );
    return 0;
}



