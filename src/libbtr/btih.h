// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

// See specification http://www.ietf.org/rfc/rfc4648.txt

#ifndef LIBBTR_BTIH_H
#define LIBBTR_BTIH_H

#include <stdint.h>
#include <stddef.h>

typedef struct bt_hash_t {
    uint8_t * binary;
    size_t    length;
} bt_hash;

bt_hash * bt_base32_decode ( void * ctx, char * src,     size_t src_length );
char *    bt_base32_encode ( void * ctx, bt_hash * hash, size_t * result_length );
bt_hash * bt_base64_decode ( void * ctx, char * src,     size_t src_length );
char *    bt_base64_encode ( void * ctx, bt_hash * hash, size_t * result_length );

#endif
