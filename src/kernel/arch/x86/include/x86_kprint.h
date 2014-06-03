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
#include <stdint.h>

#ifndef __X86_KPRINT_H
#define __X86_KPRINT_H 1

typedef uint32_t x86_kfpos_t;

int x86_kprintf(const char *format, ...);
int x86_kputchar(int character);
int x86_kputs(const char* str);
int x86_kgetpos(x86_kfpos_t* pos);
int x86_ksetpos(x86_kfpos_t* pos);
int x86_ktell();
int x86_kseek(long offset, int whence);
void x86_krewind();


#endif /* __X86_KPRINTF */

