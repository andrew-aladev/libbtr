#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <talloc.h>
#include <btbot/url.h>

int main() {
    TALLOC_CTX * ctx = talloc_new ( NULL );
    if ( !ctx ) {
        talloc_free ( ctx );
        return 1;
    }
    char * url;

    // should return NULL
    url = bt_unescape ( ctx, NULL );
    if ( url ) {
        talloc_free ( ctx );
        return 1;
    }
    url = bt_unescape ( ctx, "%" );
    if ( url ) {
        talloc_free ( ctx );
        return 1;
    }
    url = bt_unescape ( ctx, "%1" );
    if ( url ) {
        talloc_free ( ctx );
        return 2;
    }

    //should return unescaped url
    url = bt_unescape ( ctx, "Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29" );
    if ( !url || strcmp ( url, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ) {
        talloc_free ( ctx );
        return 3;
    }
    talloc_free ( url );

    url = bt_unescape ( ctx, "udp%3A%2F%2Ftracker.openbittorrent.com%3A80" );
    if ( !url || strcmp ( url, "udp://tracker.openbittorrent.com:80" ) ) {
        talloc_free ( ctx );
        return 4;
    }
    talloc_free ( url );

    talloc_free ( ctx );
    return 0;
}


