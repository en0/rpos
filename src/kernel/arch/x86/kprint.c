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
#include "string.h"

#define VIDEO_MEMORY 0xB8000
#define VIDEO_COLOR 0x04 // Red on black
#define TAB_WIDTH 4
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_CHAR_WIDTH 2
#define SCREEN_MAX_CURSOR (SCREEN_WIDTH * SCREEN_HEIGHT)
#define SCREEN_LAST_LINE_HOME (SCREEN_MAX_CURSOR - SCREEN_WIDTH)
#define SCREEN_REAL_WIDTH (SCREEN_WIDTH * SCREEN_CHAR_WIDTH)

typedef volatile uint16_t vidtext_t;

x86_kfpos_t _cursor = 0;
vidtext_t* _video_memory = (vidtext_t*)VIDEO_MEMORY;

int _set_color(char character, int color) {
    return (((int)character) & 0xFF) | (color << 8);
}

void _scroll() {

    void* v;
    int i;

    if (_cursor >= SCREEN_MAX_CURSOR) {

        v = (char*)VIDEO_MEMORY;

        for(i = 0; i < SCREEN_HEIGHT-1; i++, v+=SCREEN_REAL_WIDTH)
            memcpy(v, v+SCREEN_REAL_WIDTH, SCREEN_REAL_WIDTH);

        // fill last row with spaces
        for(i = 0; i < SCREEN_WIDTH; i++, v+=2) 
            *(vidtext_t*)v = _set_color(' ', VIDEO_COLOR);

        _cursor = SCREEN_LAST_LINE_HOME;
    }
}

void x86_kclear() {

    int i;

    for(i = 0; i < SCREEN_HEIGHT*2; i++)
        x86_kputchar('\n');
    x86_krewind();
}

int x86_kprintf(const char *format, ...) {
    const char *p;
    int _ret = 0;

    int va_char;
    char *va_str;

    va_list params;
    va_start(params, format);

    for(p = format; *p != '\0'; p++) {

        if(*p != '%') {
            x86_kputchar((int)*p);
            _ret++;
            continue;
        }

        switch(*++p) {
            case '%' : /* Literal % */
                x86_kputchar((int)'%');
                _ret++;
                break;

            case 'c' : /* Charater */
                va_char = va_arg(params, int);
                x86_kputchar(va_char);
                _ret++;
                break;

            case 's' : /* C String */
                va_str = (char*)va_arg(params, int);
                for(;*va_str != '\0'; va_str++) {
                    x86_kputchar((int)(*va_str));
                    _ret++;
                }
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
    int i;

    switch((char)(character & 0xFF)) {

        case '\n':
            _cursor += SCREEN_WIDTH;
        case '\r':
            _cursor -= (_cursor % SCREEN_WIDTH);
            break;

        case '\b':
            if (_cursor > 0)
                _video_memory[--_cursor] = _set_color(' ', VIDEO_COLOR);
            break;

        case '\t':
            x86_kputchar(0x20);
            for(i = 0; i < (_cursor) % TAB_WIDTH; i++)
                x86_kputchar(0x20);
            break;

        default:
            _video_memory[_cursor++] = _set_color(character, VIDEO_COLOR);
            break;
    }
    _scroll();
    return character;
}


int x86_kputs(const char* str) {
    int i;
    for(i = 1; *str != '\0'; str++, i++)
        x86_kputchar((int)*str);
    x86_kputchar('\n');
    return i;
}


int x86_ktell() {
    return _cursor;
}


int x86_kseek(long offset, int whence) {

    x86_kfpos_t nc = _cursor;

    switch(whence) {
        case X86_KSEEK_SET:
            nc = offset;
            break;
        case X86_KSEEK_CUR:
            nc += offset;
            break;
        case X86_KSEEK_END:
            nc = SCREEN_MAX_CURSOR + offset;
            break;
        default:
            // Invalid origin.
            return 1;
    }

    if(nc >= 0 && nc < SCREEN_MAX_CURSOR)
        _cursor = nc;
        return 0;

    // cursor not in range
    return -1;
}


void x86_krewind() {
    _cursor = 0;
}


#endif /** PROFILE_DEBUG **/
