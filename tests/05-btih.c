// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <talloc/helpers.h>
#include <libbtr/btih.h>

bool decode32_null ( void * ctx, char * str ) {
    return bt_base32_decode ( ctx, str, strlen ( str ) ) == NULL;
}
bool decode64_null ( void * ctx, char * str ) {
    return bt_base64_decode ( ctx, str, strlen ( str ) ) == NULL;
}
bool encode32_null ( void * ctx, bt_hash * hash ) {
    size_t result_length;
    return bt_base32_encode ( ctx, hash, & result_length ) == NULL;
}
bool encode64_null ( void * ctx, bt_hash * hash ) {
    size_t result_length;
    return bt_base64_encode ( ctx, hash, & result_length ) == NULL;
}

bool test_null ( void * ctx ) {
    bt_hash * empty_hash = talloc ( ctx, sizeof ( bt_hash ) );
    if ( empty_hash == NULL ) {
        return false;
    }
    empty_hash->binary = talloc_new ( empty_hash );
    empty_hash->length = 0;

    if (
        ! (
            decode32_null ( ctx, "" )          &&
            decode32_null ( ctx, "=" )         &&
            decode32_null ( ctx, "========" )  &&
            decode32_null ( ctx, "=a======" )  &&
            decode32_null ( ctx, "a========" ) &&

            decode64_null ( ctx, "" )      &&
            decode64_null ( ctx, "=" )     &&
            decode64_null ( ctx, "====" )  &&
            decode64_null ( ctx, "=a==" )  &&
            decode64_null ( ctx, "a====" ) &&

            encode32_null ( ctx, empty_hash ) &&
            encode64_null ( ctx, empty_hash )
        )
    ) {
        return false;
    }
    talloc_free ( empty_hash );
    return true;
}

bool decode_valid ( bt_hash * result, uint8_t hash[], size_t length ) {
    if (
        ! (
            result != NULL &&
            result->length == length &&
            !memcmp ( result->binary, hash, length )
        )
    ) {
        return false;
    }
    talloc_free ( result );
    return true;
}
bool encode_valid ( char * result, size_t result_length, char * answer ) {
    if (
        ! (
            result != NULL &&
            result_length == strlen ( answer ) &&
            !strncmp ( result, answer, result_length )
        )
    ) {
        return false;
    }
    talloc_free ( result );
    return true;
}
bool decode32_valid ( void * ctx, char * str, uint8_t hash[], size_t length ) {
    bt_hash * result = bt_base32_decode ( ctx, str, strlen ( str ) );
    return decode_valid ( result, hash, length );
}
bool decode64_valid ( void * ctx, char * str, uint8_t hash[], size_t length ) {
    bt_hash * result = bt_base64_decode ( ctx, str, strlen ( str ) );
    return decode_valid ( result, hash, length );
}
bt_hash * encode_set_hash ( void * ctx, uint8_t arr[], size_t length ) {
    bt_hash * hash = talloc ( ctx, sizeof ( bt_hash ) );
    if ( hash == NULL ) {
        return NULL;
    }
    hash->binary = arr;
    hash->length = length;
    return hash;
}
bool encode32_valid ( void * ctx, uint8_t arr[], size_t length, char * answer ) {
    bt_hash * hash = encode_set_hash ( ctx, arr, length );
    if ( hash == NULL ) {
        return false;
    }
    size_t result_length;
    char * result = bt_base32_encode ( ctx, hash, & result_length );
    talloc_free ( hash );
    if ( result == NULL ) {
        return false;
    }
    return encode_valid ( result, result_length, answer );
}
bool encode64_valid ( void * ctx, uint8_t arr[], size_t length, char * answer ) {
    bt_hash * hash = encode_set_hash ( ctx, arr, length );
    if ( hash == NULL ) {
        return false;
    }
    size_t result_length;
    char * result = bt_base64_encode ( ctx, hash, & result_length );
    talloc_free ( hash );
    if ( result == NULL ) {
        return false;
    }
    return encode_valid ( result, result_length, answer );
}

bool test_valid ( void * ctx ) {
    uint8_t hash_1[] = {'a', 'b', 'c'};
    uint8_t hash_2[] = {0x09, 0xA0, 0x75, 0x1E, 0x3D, 0xF3, 0xFC, 0x9D, 0xE8, 0xE6, 0x84, 0xC4, 0x23, 0x95, 0x00, 0xFD, 0x26, 0x7B, 0x10, 0xBE};
    uint8_t hash_3[] = {'a', 'b', 'c', 'd'};
    uint8_t hash_4[] = { 0xEF, 0x4D, 0x7C, 0x6F, 0xBE, 0x74, 0x77, 0xB6, 0xDE, 0xE7, 0x96, 0xDA, 0xE9, 0xBD, 0x39, 0x69, 0xC6, 0xB8, 0xDD, 0xD7, 0xDE, 0x6B, 0x5E, 0x1C, 0xF3, 0x9E, 0x74, 0xDB, 0x6D, 0xB9};
    if (
        ! (
            decode32_valid ( ctx, "MFRGG===", hash_1, sizeof ( hash_1 ) ) &&
            decode32_valid ( ctx, "BGQHKHR56P6J32HGQTCCHFIA7UTHWEF6", hash_2, sizeof ( hash_2 ) ) &&

            decode64_valid ( ctx, "YWJjZA==", hash_3, sizeof ( hash_3 ) ) &&
            decode64_valid ( ctx, "7018b750d7be55ba6b05aca43dfea14c85502225", hash_4, sizeof ( hash_4 ) ) &&

            encode32_valid ( ctx, hash_1, sizeof ( hash_1 ), "mfrgg===" ) &&
            encode32_valid ( ctx, hash_2, sizeof ( hash_2 ), "bgqhkhr56p6j32hgqtcchfia7uthwef6" ) &&

            encode64_valid ( ctx, hash_3, sizeof ( hash_3 ), "YWJjZA==" ) &&
            encode64_valid ( ctx, hash_4, sizeof ( hash_4 ), "7018b750d7be55ba6b05aca43dfea14c85502225" )
        )
    ) {
        return false;
    }
    return true;
}

int main () {
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        talloc_free ( ctx );
        return 1;
    }

    if ( !test_null ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }
    if ( !test_valid ( ctx ) ) {
        talloc_free ( ctx );
        return 3;
    }

    talloc_free ( ctx );
    return 0;
}



