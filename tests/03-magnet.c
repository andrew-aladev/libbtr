#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <talloc.h>
#include <btbot/magnet.h>

int main() {
    TALLOC_CTX * ctx = talloc_new ( NULL );
    if ( !ctx ) {
        talloc_free ( ctx );
        return 1;
    }
    bt_magnet_info * info;

    // should return NULL
    info = bt_magnet_parse ( ctx, NULL );
    if ( info ) {
        talloc_free ( ctx );
        return 2;
    }
    info = bt_magnet_parse ( ctx, "http:" );
    if ( info ) {
        talloc_free ( ctx );
        return 3;
    }
    info = bt_magnet_parse ( ctx, "magnet:?" );
    if ( info ) {
        talloc_free ( ctx );
        return 4;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt" );
    if ( info ) {
        talloc_free ( ctx );
        return 4;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=" );
    if ( info ) {
        talloc_free ( ctx );
        return 5;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:" );
    if ( info ) {
        talloc_free ( ctx );
        return 6;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:7018b750d7be55ba6b05" );
    if ( info ) {
        talloc_free ( ctx );
        return 7;
    }
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:tree:tiger:7N5OAMRNGMSSEUE3ORHOKWN4WWIQ5X4EBOOTLJY" );
    if ( info ) {
        talloc_free ( ctx );
        return 8;
    }

    // should return valid info
    info = bt_magnet_parse ( ctx, "magnet:?xt=urn:btih:7018b750d7be55ba6b05aca43dfea14c85502225" );
    if ( !info || strcmp ( info->hash, "7018b750d7be55ba6b05aca43dfea14c85502225" ) ) {
        talloc_free ( ctx );
        return 9;
    }
    talloc_free ( info );

    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:7018b750d7be55ba6b05aca43dfea14c85502225"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    if (
        !info ||
        info->trackers_count != 4 ||
        strcmp ( info->display_name, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ||
        strcmp ( info->trackers[0],  "udp://tracker.openbittorrent.com:80" ) ||
        strcmp ( info->trackers[1],  "udp://tracker.publicbt.com:80" ) ||
        strcmp ( info->trackers[2],  "udp://tracker.istole.it:6969" ) ||
        strcmp ( info->trackers[3],  "udp://tracker.ccc.de:80" )
    ) {
        talloc_free ( ctx );
        return 10;
    }
    talloc_free ( info );

    info = bt_magnet_parse ( ctx,
                             "magnet:?xt=urn:btih:7018b750d7be55ba6b05aca43dfea14c85502225"
                             "&dn=Gentoo+Linux+20121221+LiveDVD+-+End+Of+World+Edition+%28amd64%29"
                             "&tr.1=udp%3A%2F%2Ftracker.openbittorrent.com%3A80"
                             "&tr.2=udp%3A%2F%2Ftracker.publicbt.com%3A80"
                             "&tr.3=udp%3A%2F%2Ftracker.istole.it%3A6969"
                             "&tr.4=udp%3A%2F%2Ftracker.ccc.de%3A80" );
    if (
        !info ||
        info->trackers_count != 4 ||
        strcmp ( info->display_name, "Gentoo Linux 20121221 LiveDVD - End Of World Edition (amd64)" ) ||
        strcmp ( info->trackers[0],  "udp://tracker.openbittorrent.com:80" ) ||
        strcmp ( info->trackers[1],  "udp://tracker.publicbt.com:80" ) ||
        strcmp ( info->trackers[2],  "udp://tracker.istole.it:6969" ) ||
        strcmp ( info->trackers[3],  "udp://tracker.ccc.de:80" )
    ) {
        talloc_free ( ctx );
        return 11;
    }
    talloc_free ( info );

    talloc_free ( ctx );
    return 0;
}


