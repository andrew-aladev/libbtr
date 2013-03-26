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
    bt_base32_to_hex ( ctx, "abcdefghijklmnopqrstuvwxqz234567" );

    talloc_free ( ctx );
    return 0;
}


