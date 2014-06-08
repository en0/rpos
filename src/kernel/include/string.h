/**
 ** Copyright (c) 2014 "Ian Laird"
 ** Research Project Operating System (rpos) - https://github.com/en0/rpos
 ** 
 ** This file is part of rpos
 ** 
 ** rpos is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef __STRING_H
#define __STRING_H 1

#include <stddef.h>


/** Copy block of memory                                            **
 ** Copies the values of num bytes from the location pointed by     **
 ** source directly to the memory block pointed by destination.     **
 **                                                                 **
 ** Arguments:                                                      **
 **   destination - Pointer to the destination array where the      **
 **                 content is to be copied, type-casted to a       **
 **                 pointer of type void*.                          **
 **   source      - Pointer to the source of data to be copied,     **
 **                 type-casted to a pointer of type const void*.   **
 **   num         - Number of bytes to copy.                        **
 **                                                                 **
 ** Returns:                                                        **
 **   destination is returned.                                      **/

void *memcpy(void *destination, const void *source, size_t num);


/** Fill block of memory                                            **
 ** Sets the first num bytes of the block of memory pointed by ptr  **
 ** to the specified value (interpreted as an unsigned char).       **
 **                                                                 **
 ** Arguments:                                                      **
 **   ptr   - Pointer to the block of memory to fill.               **
 **   value - Value to be set. The value is passed as an int, but   **
 **           the function fills the block of memory using the      **
 **           unsigned char conversion of this value.               **
 **   num   - Number of bytes to be set to the value.               **
 **                                                                 **
 ** Returns:                                                        **
 **   ptr is returned.                                              **/

void *memset(void* ptr, int value, size_t num);


/** Get string length                                               **
 ** Returns the length of the C string str.                         **
 **                                                                 ** 
 ** Arguments:                                                      **
 **   str - C string                                                **
 **                                                                 **
 ** Returns:                                                        **
 **   The length of a string.                                       **/

size_t strlen(const char* str);


#endif /** __STRING_H **/

