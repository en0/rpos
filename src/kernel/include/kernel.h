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

#ifndef __KERNEL_H
#define __KERNEL_H 1

#include <stdint.h>
#include <stdbool.h>

/* Provided by linker */
extern void* _kernel;
extern void* _ekernel;

/* Provided by arch/start.s */
extern void kabort(const char*);

#define PHYS_MMAP_KERNEL    ((void*)&_kernel - 0xc0000000)
#define PHYS_MMAP_EKERNEL   ((void*)&_ekernel - 0xC0000000)

#define MMAP_PGPDE     ((void*)0xFFFFF000)
#define MMAP_PGPTE     ((void*)0xFFC00000)
#define MMAP_PMEM      ((void*)0xFFBE0000)
#define MMAP_STACK     ((void*)0xFF3E0000)
//#define MMAP_HEAP      ((void*)0xC0400000)
#define MMAP_HEAP      ((void*)&_ekernel)
#define MMAP_KERNEL    ((void*)&_kernel)

#define MMAP_USR_STACK ((void*)0xBFFFFFFF)
#define MMAP_USR_MAPS  ((void*)0x40000000)
#define MMAP_USR_TEXT  ((void*)0x08048000)

#endif /* __KERNEL_H */

