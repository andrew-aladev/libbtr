// This file is part of libbtr. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// libbtr is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// libbtr is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with libbtr. If not, see <http://www.gnu.org/licenses/>.

#include "list.h"

extern inline
bt_list * bt_list_new ( void * ctx );

extern inline
uint8_t bt_list_append ( bt_list * list, void * data );

extern inline
uint8_t bt_list_free ( void * current_list );

extern inline
size_t bt_list_get_length ( bt_list * list );