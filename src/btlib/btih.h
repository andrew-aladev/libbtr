// This file is part of btlib. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// btlib is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// btlib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with btlib. If not, see <http://www.gnu.org/licenses/>.

// See specification http://www.ietf.org/rfc/rfc4648.txt

#ifndef BTIH_H
#define BTIH_H

#include <stdint.h>
#include <talloc.h>

uint8_t * bt_base32_decode ( TALLOC_CTX * ctx, char * src,    size_t src_length, size_t * result_length );
char *    bt_base32_encode ( TALLOC_CTX * ctx, uint8_t * src, size_t src_length, size_t * result_length );
uint8_t * bt_base64_decode ( TALLOC_CTX * ctx, char * src,    size_t src_length, size_t * result_length );
char *    bt_base64_encode ( TALLOC_CTX * ctx, uint8_t * src, size_t src_length, size_t * result_length );

#endif
