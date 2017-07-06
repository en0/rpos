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

#ifndef __X86_PMEM_H
#define __X86_PMEM_H 1

#include <stdint.h>
#include <stdbool.h>

#define x86_PMEM_PAGE_SIZE 0x1000

#define x86_pmem_page_count(kb) (kb>>7)
#define x86_pmem_page_size(kb) ((kb>>7) * sizeof(uint32_t))

void* x86_pmem_alloc();
void* x86_pmem_alloc_low();
void x86_pmem_free(void* addr);
void x86_pmem_lock_region(void* addr, uint32_t size);
void x86_pmem_free_region(void* addr, uint32_t size);
void* x86_pmem_map_end(void* map_start, uint32_t size);
void x86_pmem_init(void* map_start, uint32_t size);

#endif /* __X86_PMEM_H */

