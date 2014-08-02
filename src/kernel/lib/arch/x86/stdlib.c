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

char* itoa(int value, char* str, int base) {
    static char* num = "0123456789abcdef";
    char* ret = str;
    int sign = value;

    if (base<2 || base>16) {
        *ret='\0';
        return NULL;
    }

    if(value < 0)
        value = -value;

    do {
        *ret++ = num[value % base]; 
    } while(value/=base);

    if(sign < 0)
        *ret++ = '-';

    *ret++ = '\0';

    return strrev(str);
}

char* uitoa(unsigned value, char* str, int base) {
    static char* num = "0123456789abcdef";
    char* ret = str;

    if (base<2 || base>16) {
        *ret='\0';
        return NULL;
    }

    do {
        *ret++ = num[value % base]; 
    } while(value/=base);

    *ret++ = '\0';

    return strrev(str);
}

