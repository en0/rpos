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

#include <string.h>

void *memcpy(void *destination, const void *source, size_t num) {
    char* d = (char*)destination;
    char* s = (char*)source;
    for(;num > 0; *d++ = *s++, num--);
    return destination;
}

void *memset(void* ptr, int value, size_t num) {
    char* d = (char*)ptr;
    char v = (char)(value & 0xFF);
    for(;num > 0; *d++ = v, num--);
    return ptr;
}

size_t strlen(const char* str) {
    size_t i;
    for(i = 0; *str != '\0'; str++, i++);
    return i;
}

char* strrev(const char *str) {
    int i, j;
    char c;
    char *s = str;
    for(i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
    return str;
}

