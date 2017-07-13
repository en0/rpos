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

#ifndef __PMEM_H
#define __PMEM_H 1

#include <config.h>
#include <stdint.h>
#include <stdbool.h>

/** void* pmem_alloc();                                 **
 ** allocate a 4Kb page                                 **/

/** void* pmem_alloc_low();                             **
 ** Allocate a 4Kb page from lower memory               **/

/** void pmem_free(void* addr);                         **
 ** free a 4Kb page                                     **
 ** Arguments:                                          **
 ** - addr : The memory address to free.                **/

/** void pmem_lock_region(void* addr, uint32_t size);   **
 ** Lock a range of pages                               **
 ** Arguments:                                          **
 ** - addr : physical address, The start of the region  **
 ** - size : the size of the region.                    **/

/** void pmem_free_region(void* addr, uint32_t size);   **
 ** Free a range of pages                               **
 ** Arguments:                                          **
 ** - addr : physical address, The start of the region  **
 ** - size : the size of the region.                    **/

/** void pmem_init(void* map_start, uint32_t size);     **
 ** Initialize the physical memory manager              **
 ** Arguments:                                          **
 ** - map_start : physical address to start the map     **
 ** - size : The total size of memory in KB             **/

#ifdef ARCH_x86
#include <x86_pmem.h>
#define pmem_page_count x86_pmem_page_count
#define pmem_page_size x86_pmem_page_size
#define pmem_alloc x86_pmem_alloc
#define pmem_alloc_low x86_pmem_alloc_low
#define pmem_free x86_pmem_free
#define pmem_lock_region x86_pmem_lock_region
#define pmem_free_region x86_pmem_free_region
#define pmem_map_end x86_pmem_map_end
#define pmem_init x86_pmem_init
#endif /** ARCH_x86 **/

#endif /* __PMEM_H */

