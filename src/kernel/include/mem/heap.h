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

#ifndef __HEAP_H
#define __HEAP_H 1

#include <stdint.h>

typedef struct heap_info {
    void *high_water_mark;
    void *limit;
    uint16_t flags;
} heap_info_t;


/* heap_create(start, end, flags) : Setup a new heap
 *
 * Creates a heap that allows access to a virtual memory space.
 *
 * Arguments:
 * - start: The begin address of the virtual address space.
 * - end: The last address of the virtual address space
 * - flags: The paging attribute flags assigned when new pages are allocated.*/

heap_info_t* heap_create(void* start, void* end, uint16_t flags);


/* void* heap_kmalloc(HEAP, LEN) - Allocate space on the heap.
 *
 * Allocates physical memory and assigns it to an available address range in
 * the heap.
 *
 * Example Call:
 * uint32_t* array = heap_kmalloc(VIRT_ADDR_HEAP, sizeof(uint32_t) * array_size);
 *
 * Arguments:
 * - heap: A pointer to the heap_info structure that defines the limits
 * - len: The count of bytes to allocate.  */

void* heap_kmalloc(heap_info_t* heap, size_t len);

#endif /* __HEAP_H */
