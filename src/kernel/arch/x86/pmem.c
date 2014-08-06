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

#include <x86_pmem.h>
#include <string.h>
#include <kprint.h>

/**
 ** Physical Memory Manager
 ** These functions allow allocation of 4k pages of memory.
 ** The kernel should lock it's own region. The pmm will 
 ** protect it's own memory space.
 **
 ** All pages are on the 4Kb boundary. If a void* is freed
 ** that is not on a page boundary, it will be assumed that
 ** the page boundary was intended and free the page for which
 ** it is a member.
 **
 ** Freeing a region will only free ranges that contain an entire
 ** page. partial refrences will not be freed.
 **
 ** Locking a region will lock an page touched by a range. if a 
 ** partial page is refrenced then it will be locked.
 **/

/** Macros to interact with the memory map. **/

#define PAGE_PER_INDEX 32
#define PAGE_BIT_FRAME 12

#define LOCK_PAGE(map, index, offset) (map[index] |= (1<<offset))
#define FREE_PAGE(map, index, offset) (map[index] &= ~(1<<offset))

#define GET_PAGE_INDEX(page) (page / PAGE_PER_INDEX)
#define GET_PAGE_OFFSET(page) (page % PAGE_PER_INDEX)

#define IS_PAGE_LOCKED(block, offset) (block & (1<<offset))
#define GET_PAGE(index, offset) ((index * PAGE_PER_INDEX) + offset)

#define TO_PAGE(addr) (((uint32_t)addr)>>PAGE_BIT_FRAME)
#define TO_ADDR(page) ((void*)(page<<PAGE_BIT_FRAME))

/** Globals **/

void* _map_start;
void* _map_end;
uint32_t* _map;
uint32_t _map_size;
uint32_t _map_start_page;
uint32_t _map_end_page;

/** Prototypes of private functions **/

void _lock_page(uint32_t page);
void _free_page(uint32_t page);
void* _alloc(uint32_t start);
uint32_t _first_free_offset(uint32_t block);


/** Private Functions **/

uint32_t _first_free_offset(uint32_t block) {
    int i;
    for(i = 0; i < 32; i++)
        if(!IS_PAGE_LOCKED(block, i))
            return i;
    return ~0;
}

void _lock_page(uint32_t page) {
    uint32_t index = GET_PAGE_INDEX(page),
             offset = GET_PAGE_OFFSET(page);
    LOCK_PAGE(_map, index, offset);
}

void _free_page(uint32_t page) {
    uint32_t index = GET_PAGE_INDEX(page),
             offset = GET_PAGE_OFFSET(page);

    /** We need to protect the map memory **/
    if(page < _map_start_page || page > _map_end_page)
        FREE_PAGE(_map, index, offset);
}


/** Public Functions **/

void* x86_pmem_map_end(void* map_start, uint32_t size) {
    return map_start + x86_pmem_page_size(size);
}

void x86_pmem_init(void* map_start, uint32_t size) {

    // map_start is the _end of the kernel
    // size is the actual size of the system memory in KB.

    _map_start = map_start;
    _map_size = x86_pmem_page_count(size);

    //_map_end = _map_start + x86_pmem_page_size(size);
    _map_end = x86_pmem_map_end(map_start, size);
    _map = (uint32_t*)_map_start;

    _map_start_page = TO_PAGE(_map_start);
    _map_end_page = TO_PAGE(_map_end);

    // Lock all regions
    memset(_map_start, 0xFF, _map_size*sizeof(uint32_t));
}

void x86_pmem_free_region(void* addr, uint32_t size) {
    uint32_t first_page = TO_PAGE(addr),
             last_page = TO_PAGE((addr+size));

    while(first_page < last_page) {
        _free_page(first_page);
        first_page++;
    }
}

void x86_pmem_lock_region(void* addr, uint32_t size) {
    uint32_t first_page = TO_PAGE(addr),
             last_page = TO_PAGE((addr+size-1));
    
    do {
        _lock_page(first_page);
        first_page++;
    }while(first_page <= last_page);
}

void* _alloc(uint32_t start) {
    uint32_t i, offset, page;
    for(i = GET_PAGE_INDEX(start); i < _map_size; i++)
        if(_map[i] < UINT32_MAX) {
            offset = _first_free_offset(_map[i]);
            page = GET_PAGE(i, offset);
            LOCK_PAGE(_map, i, offset);
            return TO_ADDR(page);
        }
    return NULL;
}

void* x86_pmem_alloc_low() { return _alloc(0x00); }
void* x86_pmem_alloc() { return _alloc(0x100); }
void x86_pmem_free(void* addr) { _free_page(TO_PAGE(addr)); }

