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

#include <config.h>

#ifdef PROFILE_DEBUG

#ifndef __KPRINT_H
#define __KPRINT_H 1

#define KSEEK_SET X86_KSEEK_SET
#define KSEEK_CUR X86_KSEEK_CUR
#define KSEEK_END X86_KSEEK_END

/** Platform independent function Information **/


/** Function: kprintf(const char *format, ...)
 ** Write the C string pointed by format to the text mode video memory.
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


/** Function: int kputchar(int character)
 ** Writes a character to the text mode video memory
 **
 ** Arguments:
 **   character - The int promotion fo the character to be written.
 **               The value is converted to an unsigned char when written
 **
 ** Returns:
 **   On Success, the value of the character being written is returned
 **   On Error, -1 is returned */


/** Function: int kputs(const char* str)
 ** Writes the C string pointed to by str to the text mode video memory and 
 ** appends a newline character ('\n')
 ** 
 ** Arguments:
 **   C String to be printed
 ** 
 ** Returns:
 **   On success, the total number of characters written is returned.
 **   On error, -1 is returned. */


/** Function: int kseek (long int offset, int origin )
 ** Sets the position indicator associated with the stream to a new position.
 ** 
 ** Arguments:
 **   offset - Number of characters to offset from origin.
 **   origin - Position used as reference for the offset. It is specified by 
 **            one of the following constants
 **            KSEEK_SET Beginning of screen
 **            KSEEK_CUR Current cursor location
 **            KSEEK_END End of screen
 **
 ** Returns:
 **   If successful, the function returns zero.
 **   Otherwise, it returns non-zero value. */


/** Function: int ktell()
 ** Returns the current value of the position indicator of the stream.
 **
 ** Returns:
 **   On success, the current value of the position indicator is returned.
 **   On failure, -1 is returned */


/** Function: void krewind()
 ** Sets the position indicator associated with stream to the beginning of 
 ** the file. */


/** 
 ** Setup macros to the real functions from the arch specific implementation 
 **/

#ifdef ARCH_x86
#include <x86_kprint.h>
#define kclear x86_kclear
#define kfpos x86_kfpos
#define kprintf x86_kprintf
#define kputchar x86_kputchar
#define kputs x86_kputs
#define ktell x86_ktell
#define kseek x86_kseek
#define krewind x86_krewind
#endif /** ARCH_x86 **/


#endif /* __KPRINTF */

#endif /* PROFILE_DEBUG */
