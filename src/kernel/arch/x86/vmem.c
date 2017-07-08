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
 ** These functions manage page directory structures for use with kernal and
 ** application context. With these functions, you can create new page
 ** directories bassed on the kernel context and activate it.
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
 ** vmem_activate(pdt*) will install the given page directory.
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

/*
 * Constants
 */

#define PAGE_FRAME_MASK      0b11111111111111111111000000000000
#define PAGE_ATTRS_MASK      0b00000000000000000000111111111111
#define TABLE_INDEX_MASK     0b00000000001111111111000000000000
#define TABLE_DIRECTORY_MASK 0b11111111110000000000000000000000
#define x86_FLG_SUPER_RW     (x86_FLG_VMEM_WRITABLE | x86_FLG_VMEM_SUPERVISOR)


/*
 * Macros
 */

#define GET_PAGE_TABLE_INDEX(a) ((a & TABLE_DIRECTORY_MASK)>>22)
#define GET_PAGE_INDEX(a) ((a & TABLE_INDEX_MASK)>>12)
#define GET_FLAG(entry, attr) (entry & attr)
#define SET_FLAG(entry, attr) (entry |= attr)
#define DEL_FLAG(entry, attr) (entry &= ~(attr))
#define IS_FLAG(entry, attr) (GET_FLAG(entry, attr) == attr)


/*
 * Global Variables
 */

x86_vmem_context* root_context;


/*
 * PRIVATE METHOD PROTOTYPES
 */

void _x86_map_page(x86_vmem_context*, x86_virt_addr, x86_phys_addr, uint32_t);
void _x86_vmem_enable_paging();


/*
 * PUBLIC METHODS
 */

x86_vmem_context* x86_vmem_init(x86_virt_addr* vaddr, size_t len, x86_phys_addr* paddr) {

    /*
     * vmem_init(vbase, pbase, length) should initialize the kernel space page 
     * directory and map the given region: This is to map kernel memory. This
     * function will also install the kernel pdt and turn on paging and return
     * the newly created pdt*.
     */

    // allocate space
    x86_vmem_context* ctx = root_context = x86_pmem_alloc();

    // empty all the page directory entries
    memset(ctx, 0x00, x86_PMEM_PAGE_SIZE);

    // Map the page directory to itself (since it's the root context)
    _x86_map_page(ctx, (x86_virt_addr)ctx, x86_FLG_SUPER_RW, (x86_phys_addr)ctx);

    // Map the kernel memory space.
    x86_vmem_map_region(ctx, vaddr, len, x86_FLG_SUPER_RW, paddr);

    // Install the page directory
    x86_vmem_activate(ctx);

    // Enable paging
    _x86_vmem_enable_paging();

    return ctx;
}

x86_vmem_context* x86_vmem_copy_context(x86_vmem_context* ctx) {

    /*
     * vmem_copy_context(ctx*) will create a copy of pdt* and return a new 
     * directory pointer. This new pdt* can then be used to allocate additional
     * space for the new context.
     */

    if(ctx == NULL)
        return x86_vmem_copy_context(root_context);

    return (x86_vmem_context*)NULL;
}

void x86_vmem_destroy_context(x86_vmem_context* ctx) {

    /*
     * vmm_destroy_context(ctx*) will find all allocated space owned by this
     * directory and free each page with pmem_free. Then the pdt entry will be
     * removed.
     */

    if(ctx == NULL)
        // We cannot destroy the root context.
        return;

}

x86_virt_addr* x86_vmem_kalloc(x86_vmem_context* ctx, x86_virt_addr* vaddr, size_t len, uint32_t flags) {

    /*
     * vmem_kalloc(pdt*, virt_addr, length, flags) will allocate a `length` byte
     * region of memory at `virt_addr` through pdt* directory. If virt_addr is
     * NULL, vmem will find the first fit.
     */

    if(ctx == NULL)
        return x86_vmem_kalloc(root_context, vaddr, len, flags);

    return (x86_virt_addr*)NULL;
}

void x86_vmem_kfree(x86_vmem_context* ctx, x86_virt_addr* vaddr, size_t len) {

    /*
     * vmem_kfree(pdt*, virt_addr, length) will unmap the given page entry
     * pointed to by virt_addr. If the page is not a COPY, the physical memory
     * will also be freed with pmem_free
     */

    if(ctx == NULL)
        return x86_vmem_kfree(root_context, vaddr, len);

}

void x86_vmem_map_region(x86_vmem_context* ctx, x86_virt_addr* vaddr, size_t len, uint32_t flags, x86_phys_addr* paddr) {

    /*
     * vmem_map_region(pdt*, virt_addr, phys_addr, length) will indiscriminately
     * map a virtual address to a physical address through pdt*. This is for
     * static memory locations like video memory or the bios.
     */

    if(ctx == NULL)
        return x86_vmem_map_region(root_context, vaddr, len, flags, paddr);

    x86_virt_addr vbase;
    x86_phys_addr pbase;

    for(vbase = (x86_virt_addr)vaddr & 0xFFFFF000, 
        pbase = (x86_virt_addr)paddr & 0xFFFFF000;
        vbase < (uint32_t)vaddr + len;
        _x86_map_page(ctx, vbase, pbase, flags),
        vbase += x86_PMEM_PAGE_SIZE,
        pbase += x86_PMEM_PAGE_SIZE); 
}

void x86_vmem_activate(x86_vmem_context* ctx) {

   /* 
    * will install the given page directory.
    * cr3 = ctx;
    */

    if(ctx == NULL)
        return x86_vmem_activate(root_context);

    asm("mov %0, %%cr3;"
        : // no output
        : "r"((uint32_t)ctx)
        : "%eax" );
}


/*
 * PRIVATE METHODS
 */

void _x86_map_page(x86_vmem_context* ctx, x86_virt_addr vaddr, uint32_t flags, x86_phys_addr paddr) {

    /*
     * _x86_map_page(ctx,vaddr,paddr,flags) will indiscriminately map the given
     * physical page to the given virtual address with the given flags. If a
     * new page is allocated, it will be automaticly added to the root context
     */

    x86_phys_addr* table;

    // Get the indexes for the page tables from the virtual address.
    uint32_t idx_pt = GET_PAGE_TABLE_INDEX(vaddr);
    uint32_t idx_pg = GET_PAGE_INDEX(vaddr);

    if(IS_FLAG(ctx[idx_pt], x86_FLG_VMEM_PRESENT)) {

        // Page is in memory. Get a handle to it.
        table = (x86_phys_addr*)(ctx[idx_pt] & PAGE_FRAME_MASK);

    } else {

        // Get a new page and set it to zero.
        table = (x86_phys_addr*)x86_pmem_alloc();
        memset((void*)table, 0x00, x86_PMEM_PAGE_SIZE);

        // Add the new table to the working context.
        ctx[idx_pt] = ((uint32_t)table & PAGE_FRAME_MASK) | flags | x86_FLG_VMEM_PRESENT;

        // Add this physcial address into the root context's map.
        _x86_map_page(root_context, (x86_virt_addr)table, x86_FLG_SUPER_RW, (x86_phys_addr)table);
    }

    // Set the page table target to the given physical address.
    table[idx_pg] = (paddr & PAGE_FRAME_MASK) | flags | x86_FLG_VMEM_PRESENT;
}

void _x86_vmem_enable_paging() {

    /*
     * _x86_vmem_enable_paging() will set the 2 flags in CR0 to enable paging.
     * cr0 |= 0x80000001;
     */

    asm("mov %%cr0, %%eax;"
        "or %0, %%eax;"
        "mov %%eax, %%cr0;"
        : // no output
        : "r"(0x80000001)
        : "%eax", "%ebx" );
}

/** Macros to interact with the memory map. **/
