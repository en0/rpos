/**
 ** Copyright (c) 2017 "Ian Laird"
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

#include <stdint.h>

#ifndef __DEBUG_H
#define __DEBUG_H 1

/** Function: dbg_printf(const char *format, ...)
 ** Write the C string to the serial console with the given format.

 ** If format includes format specifiers, then additional arguments following
 ** format are formatted and inserted into the resulting string, replacing
 ** their respective specifiers.
 **
 ** Arguments:
 **   format - C string that contains the text to be written.
 **            Can optionally contain embedded format specifiers that are
 **            replaced by the values specified in subsequent arguments
 **            and formatted as requested.
 **   ...    - Depending on the format string, the function may expect a
 **
 **            sequence of additional arguments. Each containing a value to be 
 **            used to replace a format specifier in the format string.
 **
 ** Returns:
 **   On success, the total number of characters written is returned.
 **   On error, -1 is returned. */

int dbg_printf(const char *format, ...);

/** Function: int dbg_putchar(int character)
 ** Writes a character to the serial console
 **
 ** Arguments:
 **   character - The int promotion fo the character to be written.
 **               The value is converted to an unsigned char when written
 **
 ** Returns:
 **   On Success, the value of the character being written is returned
 **   On Error, -1 is returned */

int dbg_putchar(int character);

/** Function: int dbg_puts(const char* str)
 ** Writes the C string pointed to by str to the serial console and
 ** appends a newline character ('\n')
 ** 
 ** Arguments:
 **   C String to be printed
 ** 
 ** Returns:
 **   On success, the total number of characters written is returned.
 **   On error, -1 is returned. */

int dbg_puts(const char* str);

/** Setup the hardware to use for writing.  */

void initDEBUG();

#endif /* __DEBUG */

