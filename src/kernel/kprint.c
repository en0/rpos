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

#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <kernel.h>

#define VIDEO_MEMORY VIRT_ADDR_VGA3
#define VIDEO_COLOR 0x04 // Red on black - Easy to read
//#define VIDEO_COLOR 0x80 // black on gray
#define TAB_WIDTH 4
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_CHAR_WIDTH 2
#define SCREEN_MAX_CURSOR (SCREEN_WIDTH * SCREEN_HEIGHT)
#define SCREEN_LAST_LINE_HOME (SCREEN_MAX_CURSOR - SCREEN_WIDTH)
#define SCREEN_REAL_WIDTH (SCREEN_WIDTH * SCREEN_CHAR_WIDTH)

typedef volatile uint16_t vidtext_t;
typedef uint32_t kfpos_t;

kfpos_t _cursor = 0;
vidtext_t* _video_memory = (vidtext_t*)VIDEO_MEMORY;

extern char* itoa(int value, char* str, int base);
extern char* uitoa(int value, char* str, int base);

int _set_color(char character, int color) {
    return (((int)character) & 0xFF) | (color << 8);
}

void _scroll() {

    void* v;
    int i;

    if (_cursor >= SCREEN_MAX_CURSOR) {

        v = (char*)_video_memory;

        for(i = 0; i < SCREEN_HEIGHT-1; i++, v+=SCREEN_REAL_WIDTH)
            memcpy(v, v+SCREEN_REAL_WIDTH, SCREEN_REAL_WIDTH);

        // fill last row with spaces
        for(i = 0; i < SCREEN_WIDTH; i++, v+=2) 
            *(vidtext_t*)v = _set_color(' ', VIDEO_COLOR);

        _cursor = SCREEN_LAST_LINE_HOME;
    }
}

void kclear() {

    int i;

    for(i = 0; i < SCREEN_HEIGHT*2; i++)
        kputchar('\n');
    _cursor = 0;
}

int kprintf(const char *format, ...) {
    const char *p;
    int _ret = 0;

    int va_char;
    char *va_str;
    char buffer[32];

    va_list params;
    va_start(params, format);

    for(p = format; *p != '\0'; p++) {

        if(*p != '%') {
            kputchar((int)*p);
            _ret++;
            continue;
        }

        switch(*++p) {
            case '%' : /* Literal % */
                kputchar((int)'%');
                _ret++;
                break;

            case 'c' : /* Charater */
                va_char = va_arg(params, int);
                kputchar(va_char);
                _ret++;
                break;

            case 's' : /* C String */
                va_str = (char*)va_arg(params, int);
                for(;*va_str != '\0'; va_str++) {
                    kputchar((int)(*va_str));
                    _ret++;
                }
                break;

            case 'd' : /* Signed Integer */
            case 'i' :
                va_char = (int)va_arg(params, int);
                itoa(va_char, buffer, 10);
                kprintf(buffer);
                break;

            case 'u' : /* Unsigned Integer */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 10);
                kprintf(buffer);
                break;

            case 'x' : /* Unsigned Hex (lower case only) */
            case 'X' : 
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 16);
                kprintf(buffer);
                break;
                
            case 'o' : /* Unsigned Octal */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 8);
                kprintf(buffer);
                break;

            case 'p' : /* Pointer */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 16);
                kprintf("0x%s", buffer);
                break;
            
            case 'b' : /* Binary */
                va_char = (int)va_arg(params, int);
                uitoa(va_char, buffer, 2);
                kprintf("%sb", buffer);

            case 'n' : /* nothing (Param must be an int) */
                va_char = (int)va_arg(params, int);
                break;
        }
    }
    
    va_end(params);
    return 0;
}


int kputchar(int character) {
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
            kputchar(0x20);
            for(i = 0; i < (_cursor) % TAB_WIDTH; i++)
                kputchar(0x20);
            break;

        default:
            _video_memory[_cursor++] = _set_color(character, VIDEO_COLOR);
            break;
    }
    _scroll();
    return character;
}


int kputs(const char* str) {
    int i;
    for(i = 1; *str != '\0'; str++, i++)
        kputchar((int)*str);
    kputchar('\n');
    return i;
}
