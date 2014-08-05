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

#include <config.h>
#include <kernel.h>
#include <multiboot.h>
#include <kprint.h>
#include <string.h>

#define CHECK_FLAG(a,b) ((a & b) == b)

bool validate_boot_env(multiboot_info_t* mbi) {

    /** 
     ** Required Information
     ** 1. lower/upper memory info
     ** 2. Full Memory Map
     ** 3. Either modules OR boot device with drive info
     **
     ** Optional Items 
     ** 1. Command Line
     ** 2. Boot Device **
     **    a. Drive Info
     ** 3. Modules **
     **
     ** Ignore Items
     ** 1. Symbol Table
     ** 2. Config Table
     ** 3. Boot Loader Name
     ** 4. APM Table ??
     ** 5. Video Info ??
     **/

    /** MBOOT Sanity Check **/
    if( CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_AOUT_SYMS) && 
        CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_ELF_SHDR)) {
        kprintf("ERROR: Multiboot data is not sane!\n");
        return false;
    }

    /** Need memory info **/
    if( !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MEM_MAP) ||
        !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MEMORY)) {
        kprintf("ERROR: No memory information provided by boot loader!\n");
        return false;
    }

    /** Need a boot device **/
    if( !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MODS) &&
      ( !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_BOOTDEV) ||
        !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_DRIVE_INFO))) {
        kprintf("ERROR: No boot device!\n");
        return false;
    }

    return true;
}

void init_pmem(multiboot_info_t* mbi) {
    kprintf("Detected total memory: %i Kb\n", (mbi->mem_upper + mbi->mem_lower));

    pmem_init(_END, (mbi->mem_upper + mbi->mem_lower));

    multiboot_memory_map_t *mmap;
    for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        if(mmap->type == 1) {
            kprintf("Freeing memory block: %p, length: 0x%x\n", (uint32_t)mmap->addr, (uint32_t)mmap->len);
            pmem_free_region((void*)((uint32_t)mmap->addr), (uint32_t)mmap->len);
        }
    }

    pmem_lock_region(_START, (_END - _START));
    pmem_lock_region(0x00, 4096);
}

void main(multiboot_info_t* bootinfo) {
    /* bootinfo points to the multiboot header
     * end_of_kernel points to the bottom of the stack
     * We need to lock memory to end_of_kernel and set up a new stack */

    if(!validate_boot_env(bootinfo)) return;
    init_pmem(bootinfo);
    kprintf("Research Porject Kernel\n"
            "Kernel starts at %p\n"
            "Kernel ends at %p\n",
            &_start, &_end);

    kprintf("Attempting to allocate some memory...\n");
    void* m = pmem_alloc();
    char* x = (char*)m;
    memcpy(m, "Hello, World!\0", 15);

    if(m == NULL) kprintf("Didnt work\n");
    else {
        kprintf("Data stored in [%p]: %s", m, x);
    }

    for(;;);
}

