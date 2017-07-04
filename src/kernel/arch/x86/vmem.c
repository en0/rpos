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
#include <x86_pmem.h>
#include <string.h>
#include <kprint.h>

/**
 ** Virtual Memory Manager
 ** We are still figuring out how this works.
 **/

/* Proto type */
void _x86_map_page(x86_virt_addr, x86_phys_addr, uint32_t);
x86_phys_addr* _x86_allocate_page_table(uint32_t, uint32_t);
x86_phys_addr* _x86_get_phys_addr(uint32_t*, uint32_t);

#define PAGE_FRAME_MASK      0b11111111111111111111000000000000
#define PAGE_ATTRS_MASK      0b00000000000000000000001111111111
#define TABLE_INDEX_MASK     0b00000000001111111111000000000000
#define TABLE_DIRECTORY_MASK 0b11111111110000000000000000000000

#define GET_DIRECTORY_INDEX(a) ((a & TABLE_DIRECTORY_MASK)>>22)
#define GET_TABLE_INDEX(a) ((a & TABLE_INDEX_MASK)>>12)

#define GET_FLAG(entry, attr) (entry & attr)
#define SET_FLAG(entry, attr) (entry |= attr)
#define DEL_FLAG(entry, attr) (entry &= ~(attr))
#define IS_FLAG(entry, attr) (GET_FLAG(entry, attr) == attr)

#define PDE_SIZE 1024
#define PDT_SIZE (sizeof(pdt_entry_t) * PDE_SIZE)

typedef uint32_t pdt_entry_t;

uint32_t* _pdt;

void* x86_vmem_init() {

    // Allocate physical memory to hold the page directory table.
    _pdt = x86_pmem_alloc();

    // write zeros to memory so that all pages are NOT present.
    memset(_pdt, 0x00, PDT_SIZE);

    // Map virtual to physcal address for the PDT location.
    x86_vmem_mmap((x86_virt_addr)_pdt, PDT_SIZE, 0, (x86_phys_addr*)_pdt);

    return (void*)_pdt;
}

x86_virt_addr* x86_vmem_mmap(x86_virt_addr vaddr, size_t length, uint32_t flags, x86_phys_addr* paddr) {

    x86_virt_addr vbase;
    x86_phys_addr pbase;

    // if(vaddr == null)
    //   vaddr = find_first_fit(pageCount);
    // else

    // Loop over the pages in the given address range and map each.
    for(vbase = vaddr & 0xFFFFF000, pbase = (x86_phys_addr)paddr & 0xFFFFF000;
        vbase < vaddr + length;
        _x86_map_page(vbase, pbase, flags), vbase+=0x1000, pbase+=0x1000);

    return (x86_virt_addr*)(vaddr & 0xFFFFF000);
}

void _x86_map_page(x86_virt_addr vaddr, x86_phys_addr paddr, uint32_t flags) {

    // TODO: Make this check the target so we do not reallocate a virt_addr
    // that is already in use.

    // Get the indexes for the page tables from the virtual address.
    uint32_t dIndex = GET_DIRECTORY_INDEX(vaddr);
    uint32_t tIndex = GET_TABLE_INDEX(vaddr);

    // lookup (or allocate) the page table address.
    x86_phys_addr* table = _x86_allocate_page_table(dIndex, flags);

    // Set the page table target to the given physical address.
    table[tIndex] = (paddr & PAGE_FRAME_MASK) | flags | x86_VMEM_PRESENT;

    kprintf("Mapped page: %p => %p (T:%i,P:%i)\n", vaddr, paddr, dIndex, tIndex);
}

x86_phys_addr* _x86_allocate_page_table(uint32_t index, uint32_t flags) {

    if(IS_FLAG(_pdt[index], x86_VMEM_PRESENT))
        return _x86_get_phys_addr(_pdt, index);

    void* m = x86_pmem_alloc();
    memset(m, 0x00, PDT_SIZE);

    _pdt[index] = ((uint32_t)m & PAGE_FRAME_MASK) | flags | x86_VMEM_PRESENT;
    return (x86_phys_addr*)m;
}

x86_phys_addr* _x86_get_phys_addr(uint32_t* base, uint32_t index) {
    return (x86_phys_addr*)(base[index] & PAGE_FRAME_MASK);
}

/** Macros to interact with the memory map. **/
