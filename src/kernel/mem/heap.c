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

#include <mem/heap.h>
#include <mem/vmem.h>
#include <mem/pmem.h>

static inline void* _alloc_frame(void* addr, uint16_t flags) {
    vmem_map_address(NULL, addr, flags);
    return addr;
}

heap_info_t* initHEAP(void* start, void* end, uint16_t flags) {
    heap_info_t *heap = (void*)_alloc_frame(start, flags);
    heap->high_water_mark = start + sizeof(heap_info_t) + 1;
    heap->flags = flags;
    heap->limit = end;
    return heap;
}

void *kmalloc(heap_info_t *heap, size_t len) {

    void *start  = heap->high_water_mark;
    void *end = start + len;

    if(end > heap->limit) 
        return NULL;

    heap->high_water_mark = end + 1;

    void *np, *ep;
    for(np = TOPAGE(start) + 0x1000, ep = TOPAGE(end); np <= ep; np += 0x1000)
        vmem_map_address(NULL, np, heap->flags);

    return start;
}

