// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include "keylist.h"

extern inline
bt_magnet_keylist * bt_magnet_keylist_new ( talloc_list * list );

extern inline
uint8_t bt_magnet_keylist_indexed ( bt_magnet_keylist * keylist, char * key, size_t key_size );

extern inline
uint8_t bt_magnet_keylist_strict ( bt_magnet_keylist * keylist );

extern inline
uint8_t bt_magnet_keylist_append ( bt_magnet_keylist * keylist, char * key, size_t key_size, char * value );
