
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

#include <config.h>

#ifdef PROFILE_DEBUG

#include <cpu.h>
#include <stdarg.h>
#include "string.h"

#define COM1 0x3f8

extern char* itoa(int value, char* str, int base);
extern char* uitoa(int value, char* str, int base);

void initDBG() {
    outb(0x00, COM1+1);
    outb(0x80, COM1+3);
    outb(0x03, COM1+0);
    outb(0x00, COM1+1);
    outb(0x03, COM1+3);
    outb(0xC7, COM1+2);
    outb(0x0B, COM1+4);
}

int is_transmit_empty() {
   return inb(COM1 + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outb(a, COM1);
}

void write_serial_string(char *str) {
    for(; *str != '\0'; write_serial(*str), str++);
}

int dbg_putchar(int character) {

    switch((char)(character & 0xFF)) {

        case '\r':
        case '\b':
            /* skip */
            break;

        default:
            write_serial(character);
            break;
    }

    return character;
}


int dbg_puts(const char* str) {
    int i;
    for(i = 1; *str != '\0'; str++, i++)
        dbg_putchar((int)*str);
    dbg_putchar('\n');
    return i;
}

int dbg_printf(const char *format, ...) {

    const char *p;
    int _ret = 0;

    int va_char;
    char *va_str;
    char buffer[32];

    va_list params;
    va_start(params, format);

    for(p = format; *p != '\0'; p++) {
        if(*p != '%') {
            dbg_putchar((int)*p);
            _ret++;
            continue;
        }

        switch(*++p) {
            case '%' : /* Literal % */
                dbg_putchar((int)'%');
                _ret++;
                break;

            case 'c' : /* Charater */
                va_char = va_arg(params, int);
                dbg_putchar(va_char);
                _ret++;
                break;

            case 's' : /* C String */
                va_str = (char*)va_arg(params, int);
                for(;*va_str != '\0'; va_str++) {
                    dbg_putchar((int)(*va_str));
                    _ret++;
                }
                break;

            case 'd' : /* Signed Integer */
            case 'i' :
                va_char = (int)va_arg(params, int);
                itoa(va_char, buffer, 10);
                dbg_printf(buffer);
                break;

            case 'u' : /* Unsigned Integer */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 10);
                dbg_printf(buffer);
                break;

            case 'x' : /* Unsigned Hex (lower case only) */
            case 'X' : 
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 16);
                dbg_printf(buffer);
                break;
                
            case 'o' : /* Unsigned Octal */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 8);
                dbg_printf(buffer);
                break;

            case 'p' : /* Pointer */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 16);
                dbg_printf("0x%s", buffer);
                break;
            
            case 'b' : /* Binary */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 2);
                dbg_printf("%sb", buffer);

            case 'n' : /* nothing (Param must be an int) */
                va_char = (int)va_arg(params, int);
                break;
        }
    }
    
    va_end(params);
    return 0;
}

#else /** PROFILE_DEBUG **/

/* 
 * If debugging is disabled, the start.c will still try to initialize the debug
 * console. Here we provide a stub to answer that call. All other refrences to
 * dbg_* commands should be wrapped in a #ifdef PROFILE_DEBUG macro.
 */

static void initDBG() {}

#endif /** PROFILE_DEBUG **/
