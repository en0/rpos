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

#include <x86_kprint.h>
#include <stdarg.h>

#define VIDEO_MEMORY 0xB8000
#define VIDEO_COLOR 0x04 // Red on black
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef volatile uint16_t vidtext_t;

x86_kfpos_t _cursor = 0;
vidtext_t* _video_memory = (vidtext_t*)VIDEO_MEMORY;

int x86_kprintf(const char *format, ...) {
    const char *p;
    int _ret = 0;

    va_list params;
    va_start(params, format);

    for(p = format; *p != '\0'; p++) {
        _ret++;

        if(*p != '%') {
            x86_kputchar((int)*p);
            continue;
        }

        switch(*++p) {
            case '%' : /* Literal % */
                x86_kputchar((int)'%');
                break;

            case 'c' : /* Charater */
                break;

            case 's' : /* C String */
                break;

            case 'd' : /* Signed Integer */
            case 'i' :
                break;

            case 'u' : /* Unsigned Integer */
                break;

            case 'x' : /* Unsigned Hex (lower case) */
                break;

            case 'X' : /* Unsigned Hex (lower case) */
                break;
                
            case 'o' : /* Unsigned Octal */
                break;

            case 'p' : /* Pointer */
                break;

            case 'n' : /* nothing (Param must be an int) */
                break;
        }
    }
    
    va_end(params);
    return 0;
}


int x86_kputchar(int character) {
    _video_memory[_cursor++] = (character & 0xFF) | (VIDEO_COLOR << 8);
    return 0;
}


int x86_kputs(const char* str) {
    for(; *str != '\0'; str++)
        x86_kputchar((int)*str);
    return 0;
}


int x86_kgetpos(x86_kfpos_t* pos) {
    return 0;
}

int x86_ksetpos(x86_kfpos_t* pos) {
    return 0;
}

int x86_ktell() {
    return 0;
}

int x86_kseek(long offset, int whence) {
    return 0;
}

void x86_krewind() {
}


#endif /** PROFILE_DEBUG **/
