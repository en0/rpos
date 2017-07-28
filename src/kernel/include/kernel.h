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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <mem/heap.h>

/* Provided by linker */
extern void* _kernel;
extern void* _ekernel;

/* Provided by arch/start.s */
extern void kabort(const char*);

// Memory layout address
//------------------------------------------------
#define PHYS_ADDR_KSTART    ((void*)&_kernel - 0xc0000000)
#define PHYS_ADDR_KEND      ((void*)&_ekernel - 0xc0000000)
#define PHYS_ADDR_VGA3      ((void*)0x000B8000)

//------------------------------------------------
#define VIRT_ADDR_PGPDE     ((void*)0xFFFFF000)
#define VIRT_ADDR_PGPTE     ((void*)0xFFC00000)
#define VIRT_ADDR_STACK     ((void*)0xFFBFFFFF)
#define VIRT_ADDR_ESTACK    ((void*)0xFF400000)
#define VIRT_ADDR_EHEAP     ((void*)0xFF3FFFFF)
#define VIRT_ADDR_HEAP      ((void*)0xC0400000)
#define VIRT_ADDR_RAMDISK   ((void*)((uint32_t)(&_ekernel + 0x1000) & 0xFFFFF000))
#define VIRT_ADDR_KEND      ((void*)&_ekernel)
#define VIRT_ADDR_KSTART    ((void*)&_kernel)
#define VIRT_ADDR_VGA3      ((void*)0xc00B8000)
//------------------------------------------------
#define VIRT_ADDR_USR_STACK ((void*)0xBFFFFFFF)
#define VIRT_ADDR_USR_MAPS  ((void*)0x40000000)
#define VIRT_ADDR_USR_TEXT  ((void*)0x08048000)

#define KERNEL_SIZE         ((size_t)(PHYS_ADDR_KEND - PHYS_ADDR_KSTART))
#define KERNEL_HEAP         ((heap_info_t *)VIRT_ADDR_HEAP)
#endif /* __KERNEL_H */

