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

#include <x86_vmem.h>
#include <string.h>
#include <kprint.h>

/**
 ** Virtual Memory Manager
 ** We are still figuring out how this works.
 **/

#define PTE_MASK_PRESENT        0b0000000000000000000000000000001
#define PTE_MASK_WRITABLE       0b0000000000000000000000000000010
#define PTE_MASK_USER           0b0000000000000000000000000000100
#define PTE_MASK_WRITETHROUGH   0b0000000000000000000000000001000
#define PTE_MASK_NOT_CACHEABLE  0b0000000000000000000000000010000
#define PTE_MASK_ACCESSED       0b0000000000000000000000000100000
#define PTE_MASK_DIRTY          0b0000000000000000000000001000000
#define PTE_MASK_PAT            0b0000000000000000000000010000000
#define PTE_MASK_CPU_GLOBAL     0b0000000000000000000000100000000
#define PTE_MASK_LV4_GLOBAL     0b0000000000000000000001000000000
#define PTE_MASK_FRAME          0b1111111111111111111000000000000

typedef uint32_t pt_entry;

//void pt_entry_add_attrib (pt_entry* e, uint32_t attrib);
//void pt_entry_del_attrib (pt_entry* e, uint32_t attrib);
//void pt_entry_set_frame (pt_entry*, physical_addr);
//bool pt_entry_is_present (pt_entry e);
//bool pt_entry_is_writable (pt_entry e);
//physical_addr pt_entry_pfn (pt_entry e);

/** Macros to interact with the memory map. **/
