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

/**
 ** Virtual Memory Manager
 ** We are still figuring out how this works but here is the plan
 **
 ** vmem_init(vbase, pbase, length) should initialize the kernel space page 
 ** directory and map the given region: This is to map kernel memory. This
 ** function will also install the kernel pdt and turn on paging and return
 ** the newly created pdt*.
 **
 ** vmem_copy_context(ctx*) will create a copy of pdt* and return a new 
 ** directory pointer. This new pdt* can then be used to allocate additional
 ** space for the new context.
 **
 ** vmm_destroy_context(ctx*) will find all allocated space owned by this
 ** directory and free each page with pmem_free. Then the pdt entry will be
 ** removed.
 **
 ** vmem_kalloc(pdt*, virt_addr, length, flags) will allocate a `length` byte
 ** region of memory at `virt_addr` through pdt* directory. If virt_addr is
 ** NULL, vmem will find the first fit.
 **
 ** vmem_kfree(pdt*, virt_addr, length) will unmap the given page entry
 ** pointed to by virt_addr. If the page is not a COPY, the physical memory
 ** will also be freed with pmem_free
 **
 ** vmem_map_region(pdt*, virt_addr, phys_addr, length) will indiscrimatly
 ** map a virtual address to a physical address through pdt*. This is for
 ** static memory locations like video memory or the bios.
 **
 ** Add a few os specific flags for things like flagging a page that should
 ** be deleted when a pdt is destroyed.
 ** - x86_VMEM_COPY - when we do the copy we can mask this bit out.
 **
 ** NOTE: If additional kernel memory is needed, it can be allocated but any
 ** directories created before this allocation will not have them mapped in
 ** there copy. This could be problematic depending on what steps the context
 ** switching goes through.
 **
 ** Everything that exists right now was just to understand the way paging
 ** worked and how to set it up.
 **/

#include <x86_vmem.h>
#include <x86_pmem.h>
#include <string.h>
#include <kprint.h>

x86_vmem_context* x86_vmem_init(x86_virt_addr* vaddr, x86_phys_addr* paddr, size_t len) {

    /*
     * vmem_init(vbase, pbase, length) should initialize the kernel space page 
     * directory and map the given region: This is to map kernel memory. This
     * function will also install the kernel pdt and turn on paging and return
     * the newly created pdt*.
     */

    x86_vmem_context* ctx = x86_pmem_alloc();
    x86_virt_addr vbase, pbase;
    uint32_t flags = x86_FLG_VMEM_PRESENT | x86_FLG_VMEM_WRITABLE | x86_FLG_VMEM_SUPERVISOR;

    memset((void*)ctx, 0x00, x86_PMEM_PAGE_SIZE);

    // Loop over the pages in the given address range and map each.
    for(vbase = (x86_virt_addr)vaddr & 0xFFFFF000, 
        pbase = (x86_virt_addr)paddr & 0xFFFFF000;
        vbase < (uint32_t)vaddr + len;
        _x86_map_page(ctx, vbase, pbase, flags),
        vbase += x86_PMEM_PAGE_SIZE,
        pbase += x86_PMEM_PAGE_SIZE); 

    return (x86_vmem_context*)ctx;
}

x86_vmem_context* x86_vmem_copy_context(x86_vmem_context* ctx) {

    /*
     * vmem_copy_context(ctx*) will create a copy of pdt* and return a new 
     * directory pointer. This new pdt* can then be used to allocate additional
     * space for the new context.
     */

    return (x86_vmem_context*)NULL;
}

void x86_vmem_destroy_context(x86_vmem_context* ctx) {

    /*
     * vmm_destroy_context(ctx*) will find all allocated space owned by this
     * directory and free each page with pmem_free. Then the pdt entry will be
     * removed.
     */

}

x86_virt_addr* x86_vmem_kalloc(x86_vmem_context* ctx, x86_virt_addr* vaddr, size_t len, uint32_t flags) {

    /*
     * vmem_kalloc(pdt*, virt_addr, length, flags) will allocate a `length` byte
     * region of memory at `virt_addr` through pdt* directory. If virt_addr is
     * NULL, vmem will find the first fit.
     */

    return (x86_virt_addr*)NULL;
}

void x86_vmem_kfree(x86_vmem_context* ctx, x86_virt_addr* vaddr, size_t len) {

    /*
     * vmem_kfree(pdt*, virt_addr, length) will unmap the given page entry
     * pointed to by virt_addr. If the page is not a COPY, the physical memory
     * will also be freed with pmem_free
     */

}

void x86_vmem_map_region(x86_vmem_context* ctx, x86_virt_addr* vaddr, x86_phys_addr* paddr, size_t len) {

    /*
     * vmem_map_region(pdt*, virt_addr, phys_addr, length) will indiscriminately
     * map a virtual address to a physical address through pdt*. This is for
     * static memory locations like video memory or the bios.
     */

}

/** PRIVATE METHODS **********************************************************/

void _x86_map_page(x86_vmem_context* ctx, x86_virt_addr vaddr, x86_phys_addr paddr, uint32_t flags) {

    /*
     * _x86_map_page(ctx,vaddr,paddr,flags) will indiscriminately map the given
     * physical page to the given virtual address with the given flags.
     */

}

/*
void _x86_map_page(x86_virt_addr, x86_phys_addr, uint32_t);
x86_phys_addr* _x86_allocate_page_table(uint32_t, uint32_t);
x86_phys_addr* _x86_get_phys_addr(uint32_t*, uint32_t);
x86_virt_addr _x86_find_first(uint32_t);

#define PAGE_FRAME_MASK      0b11111111111111111111000000000000
#define PAGE_ATTRS_MASK      0b00000000000000000000111111111111
#define TABLE_INDEX_MASK     0b00000000001111111111000000000000
#define TABLE_DIRECTORY_MASK 0b11111111110000000000000000000000

#define GET_DIRECTORY_INDEX(a) ((a & TABLE_DIRECTORY_MASK)>>22)
#define GET_TABLE_INDEX(a) ((a & TABLE_INDEX_MASK)>>12)

#define GET_FLAG(entry, attr) (entry & attr)
#define SET_FLAG(entry, attr) (entry |= attr)
#define DEL_FLAG(entry, attr) (entry &= ~(attr))
#define IS_FLAG(entry, attr) (GET_FLAG(entry, attr) == attr)

#define PDT_SIZE (sizeof(pdt_entry_t) * 1024)

uint32_t* _pdt;

void* old_vmem_init() {

    // Allocate physical memory to hold the page directory table.
    _pdt = x86_pmem_alloc();

    // write zeros to memory so that all pages are NOT present.
    memset(_pdt, 0x00, PDT_SIZE);

    // Map virtual to physcal address for the PDT location.
    x86_vmem_map_region((x86_virt_addr)_pdt, PDT_SIZE, 0, (x86_phys_addr*)_pdt);

    return (void*)_pdt;
}

x86_virt_addr* x86_vmem_map_region(x86_virt_addr vaddr, size_t length, uint32_t flags, x86_phys_addr* paddr) {

    x86_virt_addr vbase;
    x86_phys_addr pbase;

    // Loop over the pages in the given address range and map each.
    for(vbase = vaddr & 0xFFFFF000, pbase = (x86_phys_addr)paddr & 0xFFFFF000;
        vbase < vaddr + length;
        _x86_map_page(vbase, pbase, flags), vbase+=PAGE_SIZE, pbase+=PAGE_SIZE);

    return (x86_virt_addr*)(vaddr & 0xFFFFF000);
}

void* x86_vmem_alloc(size_t length, uint32_t flags) {

    uint32_t pageCount = length / PAGE_SIZE;
    x86_virt_addr vbase, vaddr;

    // compute number of pages.
    if((length % PAGE_SIZE) > 0)
        pageCount++;

    kprintf("ok, allocate %i pages.", pageCount);

    // find the first fit.
    vaddr = _x86_find_first(pageCount);

    //for(vbase = vaddr; vbase < vaddr + length; vbase+=PAGE_SIZE)
        //_x86_map_page(vbase, (x86_phys_addr)x86_pmem_alloc(), flags);

    return (void*)vaddr;
}

void x86_vmem_enable() {

    //cr3 = _pdt;
    //cr0 |= 0x80000001;

    asm("mov %0, %%cr3;"
        "mov %%cr0, %%eax;"
        "or %1, %%eax;"
        "mov %%eax, %%cr0;"
        : // no output
        : "r"((uint32_t)_pdt), "r"(0x80000001)
        : "%eax" );

}

x86_virt_addr _x86_find_first(uint32_t pageCount) {
    // ???
    return 1;
}

void _x86_map_page(x86_virt_addr vaddr, x86_phys_addr paddr, uint32_t flags) {

    // TODO: Make this check the target so we do not reallocate a virt_addr
    // that is already in use.

    // Get the indexes for the page tables from the virtual address.
    uint32_t dIndex = GET_DIRECTORY_INDEX(vaddr);
    uint32_t tIndex = GET_TABLE_INDEX(vaddr);

    // lookup (or allocate) the page table address.
    x86_phys_addr* table = _x86_allocate_page_table(dIndex, flags);

    // TODO: Check if page is already used. if so, PANIC!

    // Set the page table target to the given physical address.
    table[tIndex] = (paddr & PAGE_FRAME_MASK) | flags | x86_VMEM_PRESENT;

    kprintf("Mapped page: %p => %p (T:%i,P:%i,F:%b)\n", vaddr, paddr, dIndex, tIndex,flags);
}

x86_phys_addr* _x86_allocate_page_table(uint32_t index, uint32_t flags) {

    // TODO: since pmem_alloc() locks a full 4k page, we should setup 4 page
    // directory entries.  Currently the remaning 3072 bytes is completely lost

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
*/

/** Macros to interact with the memory map. **/
